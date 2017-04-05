
#ifndef STRINGBUILDER_H
#define STIRNGBUILDER_H

namespace wiz
{
	class StringBuilder //
	{
	private:
		char* buffer_first;
		char* buffer;
		int len;
		int capacity;
	private:
		char* end() const
		{
			return buffer_first + capacity;
		}
	public:
		StringBuilder(const int buffer_size, const char* cstr = "", int len = 0) {
			buffer = (char*)malloc(sizeof(char) * (buffer_size + 1)); // 1 for '\0'
			this->len = len;
			capacity = buffer_size;
			memcpy(buffer, cstr, this->len);
			buffer[this->len] = '\0';
			buffer_first = buffer;
		}
		StringBuilder(const StringBuilder&) = delete;
		StringBuilder& operator=(const StringBuilder&) = delete;
		virtual ~StringBuilder()
		{
			free(buffer_first);
		}

		StringBuilder& Append(const char* cstr, const int len)
		{
			if (buffer + this->len + len < end())
			{
				memcpy(buffer + this->len, cstr, len);
				buffer[this->len + len] = '\0';
				this->len = this->len + len;
			}
			else {
				if (buffer_first + this->len + len < end()) {
					memmove(buffer_first, buffer, this->len);
					memcpy(buffer_first + this->len, cstr, len);
					buffer_first[this->len + len] = '\0';
					buffer = buffer_first;
					this->len = this->len + len;
				}
				else {
					char* new_buffer = (char*)malloc(sizeof(char) * (this->len + len + 1));
					memcpy(new_buffer, buffer, this->len);
					memcpy(new_buffer + this->len, cstr, len);
					new_buffer[this->len + len] = '\0';
					free(buffer);
					buffer = new_buffer;
					buffer_first = buffer;
					this->len = this->len + len;
				}
			}
			return *this;
		}
		const char* Divide(const int idx) // need to rename!l, chk idx range!
		{
			buffer[idx] = '\0';
			return buffer;
		}
		const char* Str(int* size = nullptr) {
			if (size) { *size = len; }
			return buffer;
		}

		void Clear()
		{
			len = 0;
			buffer = buffer_first;
			buffer[0] = '\0';
		}
		int size() { return len; }
		StringBuilder& LeftShift(const int offset = 1) // chk
		{
			if (offset < 1) { return *this; }

			if (buffer + offset < end()) {
				buffer = buffer + offset;
			}
			else {
				memmove(buffer_first, buffer + offset, len - offset);
				buffer = buffer_first;
				buffer[len - offset] = '\0';
			}
			len = len - offset;
			return *this;
		}
	};
}

#endif