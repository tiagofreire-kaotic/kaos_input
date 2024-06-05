//======== ======== ======== ======== ======== ======== ======== ========
///	\file
///
///	\copyright
///		Copyright (c) Kaotic Software
///
//======== ======== ======== ======== ======== ======== ======== ========

#include "os_string_string_conversion.hpp"


namespace kaos::input::backend
{
#if 0
#ifdef _WIN32
	static uintptr_t __estimate_UTF16_UTF8_failforward(char16_t const*& p_input, char16_t const* const p_end)
	{
		wchar_t const testp = *p_input;

		if(testp < 0x0080) //Level 0
		{
			return 1;
		}
		if(testp < 0x0800) //Level 1
		{
			return 2;
		}
		if(testp > 0xD7FF && testp < 0xE000)
		{
			if(	(p_end - p_input < 2)             ||
				(testp & 0xFC00)        != 0xD800 ||
				(*(p_input + 1) & 0xFC00) != 0xDC00) return 0;
			++p_input;
			return 4;
		}
		return 3;
	}

	static uintptr_t __estimate_UTF8_UTF16(char8_t const*& p_input, char8_t const* const p_end)
	{
		char8_t const testp = *p_input;
		if((testp & 0x80) == 0) //level 0
		{
			return 1;
		}

		if((testp & 0xE0) == 0xC0) //level 1
		{
			if(	p_end - p_input < 2	|| //validate size
				testp < 0xC2		|| //validate range
				(*++p_input & 0xC0) != 0x80) //validate encoding
			{
				return 0;
			}
			return 1;
		}

		char8_t const* tlocal = p_input;

		if((testp & 0xF0) == 0xE0) //level 2
		{
			if(p_end - tlocal < 3) //validate size
			{
				return 0;
			}
			char8_t const t1 = testp & 0x0F;
			char8_t const t2 = *++tlocal;
			char8_t const t2m = t2 & 0x20;
			if(
				(!t1 && !t2m)		|| //validate range
				//(t1 == 0x0D && t2m)	|| //surrogates
				(t2 & 0xC0) != 0x80	|| //validate encoding
				(*++tlocal & 0xC0) != 0x80
				)
			{
				return 0;
			}

			p_input += 2;
			return 1;
		}

		if((testp & 0xF8) == 0xF0) //level 3
		{
			char8_t const t1 = testp & 0x07;
			if((p_end - tlocal < 4) || t1 > 4) //validate size, and partial upper_range
			{
				return 0;
			}

			char8_t const t2 = *++tlocal;
			if(
				(t1 == 4 && (t2 & 0x30))	|| //validate upper_range
				(!t1 && (t2 < 0x90))		|| //validate sub-range
				(t2 & 0xC0) != 0x80			|| //validate encoding
				(*++tlocal & 0xC0) != 0x80	||
				(*++tlocal & 0xC0) != 0x80)
			{
				return 0;
			}
			p_input += 3;
			return 2;
		}
		return 0;
	}

	static void __convert_UTF8_UTF16_unsafe(char8_t const*& p_input, char16_t*& p_out)
	{
		char8_t const testp = *p_input;

		if((testp & 0x80) == 0) //level 0
		{
			*p_out = static_cast<char16_t>(testp);
		}
		else if((testp & 0xE0) == 0xC0) //level 1
		{
			*p_out =
				static_cast<char16_t>((testp & 0x1F) << 6) |
				static_cast<char16_t>(*++p_input & 0x3F);
		}
		else
		{
			char8_t const* codeStart = p_input;
			if((testp & 0xF0) == 0xE0) //level 2
			{
				p_input += 2;
				char16_t const tcode =
					static_cast<char16_t>(testp << 12) |
					static_cast<char16_t>(static_cast<char16_t>(*++codeStart & 0x3F) << 6);
				*p_out = tcode | static_cast<char16_t>(*++codeStart & 0x3F);
			}
			else //level 3
			{
				p_input += 3;
				char16_t tcode = static_cast<char16_t>(
					(static_cast<char16_t>((testp & 0x07) << 8) |
						static_cast<char16_t>(static_cast<char16_t>(*++codeStart & 0x3F) << 2)) - char16_t{0x40});

				*p_out = 0xD800 | tcode | static_cast<char16_t>(static_cast<char16_t>(*++codeStart & 0x30) >> 4);

				tcode = 0xDC00 | static_cast<char16_t>(static_cast<char16_t>(*codeStart & 0x0F) << 6);
				*++p_out = tcode | static_cast<char16_t>(*++codeStart & 0x3F);
			}
		}
	}


	static uintptr_t to_os_natural_size(std::u8string_view const p_string)
	{
		uintptr_t count = 0;
		char8_t const* pos = p_string.data();
		char8_t const* const end = pos + p_string.size();

		for(; pos < end; ++pos)
		{
			uintptr_t const res = __estimate_UTF8_UTF16(pos, end);
			if(!res)
			{
				return 0;
			}
			count += res;
		}

		return count;
	}

	static uintptr_t from_os_natural_size(std::u16string_view const p_string)
	{
		uintptr_t count = 0;
		char16_t const* pos = p_string.data();
		char16_t const* const end = pos + p_string.size();

		for(; pos < end; ++pos)
		{
			uintptr_t const tcount = __estimate_UTF16_UTF8_failforward(pos, end);
			count += tcount ? tcount: 3;
		}

		return count;
	}

	std::basic_string<core::os_char> to_os_natural_convert(std::u8string_view const p_string)
	{
		uintptr_t const res = to_os_natural_size(p_string);
		if(!res) return {};
		std::wstring output;
		output.resize(res);

		{
			char16_t* pivot = reinterpret_cast<char16_t*>(output.data());
			char8_t const* pos = p_string.data();
			char8_t const* const end = pos + p_string.size();

			for(; pos < end; ++pos, ++pivot)
			{
				__convert_UTF8_UTF16_unsafe(pos, pivot);
			}	
		}

		return output;
	}

	std::u8string from_os_natural_convert(std::basic_string_view<core::os_char> const p_string)
	{
		std::u16string_view const t_string{reinterpret_cast<char16_t const*>(p_string.data()), p_string.size()};
		uintptr_t const reqSize = from_os_natural_size(t_string);
		if(reqSize == 0) return {};

		std::u8string buff;
		buff.resize(reqSize);
		char8_t* pivot = buff.data();

		char16_t const* pos = t_string.data();
		char16_t const* const end = pos + t_string.size();

		for(; pos < end; ++pos)
		{
			char16_t const testp = *pos;

			if((testp & 0xFC00) == 0xD800)
			{
				char16_t const* pos_next = pos + 1;
				if(pos_next < end && ((*pos_next) & 0xFC00) == 0xDC00)
				{
					char32_t const refrag = (((testp & 0x03FF) << 10) | (*(++pos) & 0x03FF)) + 0x10000;
					*pivot   =  static_cast<char8_t>(refrag >> 18)                  | char8_t{0xF0};
					*++pivot = (static_cast<char8_t>(refrag >> 12) & char8_t{0x3F}) | char8_t{0x80};
					*++pivot = (static_cast<char8_t>(refrag >>  6) & char8_t{0x3F}) | char8_t{0x80};
					*++pivot = (static_cast<char8_t>(refrag      ) & char8_t{0x3F}) | char8_t{0x80};
				}
				else
				{
					*pivot   =  static_cast<char8_t>(testp >> 12)                  | char8_t{0xE0};
					*++pivot = (static_cast<char8_t>(testp >>  6) & char8_t{0x3F}) | char8_t{0x80};
					*++pivot = (static_cast<char8_t>(testp      ) & char8_t{0x3F}) | char8_t{0x80};
				}
			}
			else
			{
				if(testp < 0x0080) //Level 0
				{
					*pivot = static_cast<char8_t>(testp);
				}
				else if(testp < 0x0800) //Level 1
				{
					*pivot   = static_cast<char8_t>(testp >> 6)     | char8_t{0xC0};
					*++pivot = (static_cast<char8_t>(testp) & 0x3F) | char8_t{0x80};
				}
				else
				{
					*pivot   =  static_cast<char8_t>(testp >> 12)                  | char8_t{0xE0};
					*++pivot = (static_cast<char8_t>(testp >>  6) & char8_t{0x3F}) | char8_t{0x80};
					*++pivot = (static_cast<char8_t>(testp      ) & char8_t{0x3F}) | char8_t{0x80};
				}
			}
		}
		return buff;
	}
#else
std::basic_string<core::os_char> to_os_natural_convert(std::u8string_view const p_string)
{
	return std::string{std::string_view{reinterpret_cast<char const*>(p_string.data()), p_string.size()}};
}

std::u8string from_os_natural_convert(std::basic_string_view<core::os_char> const p_string)
{
	return std::u8string{std::u8string_view{reinterpret_cast<char8_t const*>(p_string.data()), p_string.size()}};
}
#endif // _WIN32
#endif



#ifdef _WIN32
std::u8string from_os_HID_convert(std::basic_string_view<core::os_char> p_string)
{
	for(core::os_char const tchar: p_string)
	{
		if(tchar > 0x7F)
		{
			return {};
		}
	}

	std::u8string out;
	out.resize(p_string.size());
	char8_t* pivot = out.data();

	for(core::os_char const tchar: p_string)
	{
		*(pivot++) = static_cast<char8_t>(tchar);
	}

	return out;
}


#else
std::basic_string<char8_t> from_os_HID_convert(std::basic_string_view<core::os_char> const p_string)
{
	return std::u8string{std::u8string_view{reinterpret_cast<char8_t const*>(p_string.data()), p_string.size()}};
}
#endif

} //namespace kaos::input::backend
