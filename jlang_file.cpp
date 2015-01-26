
namespace File
{

void ReadFile(
	const char* filename,
	char* out_buffer,
	int* out_len,
	int max_len)
{
	FILE* file = fopen(filename, "r");
	assert(file, "couldn't open file: %s", filename);

	int bytes_read = fread(out_buffer, 1, max_len, file);
	assert(
		feof(file),
		"file %s is too large, max is %d bytes",
		filename, max_len);

	fclose(file);

	*out_len = bytes_read;
}

}