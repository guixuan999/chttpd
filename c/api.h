#define register_var(name, type, var) do_register_var(__FILE__, name, type, var)
extern int set_vars(char *);
char* get_vars();

// better use register_var instead of do_register_var
void do_register_var(const char* src_filename, const char* var_name, const char* var_type, void* var);
