## Things to consider when using this library:

- No data is returned from wrappers.
- All of the logic occurs in underlying funcion, wrappers do copying/moving/passing
- Malloced char* that is passed to wrapper will be freed by parse_input, data has to be copied
- Main structure pointer is immutable. It is not advised to change it in wrappers. It is better to wrap it in another structure/pointer to a pointer and mutate nested data, leaving main structure unchanged.
