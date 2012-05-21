#ifndef __UTIL_H__
#define __UTIL_H__

#include "global.h"  // definition von ATTRIBUTE_INTERNAL 

#ifdef __cplusplus
extern "C" {
#endif


#define lo(x)  (unsigned char)(x & 0xFF)
#define hi(x)  (unsigned char)((x >> 8) & 0xFF)

#define l_lo(x) (unsigned char)(x)
#define l_m1(x) (unsigned char)((x) >> 8)
#define l_m2(x) (unsigned char)((x) >> 16)
#define l_hi(x) (unsigned char)((x) >> 24)

#define save_free(d) do { \
        if ((d)) \
        { \
            mhs_free((d)); \
            (d) = NULL; \
        } \
    } while(0)


    void strlwc(char* str);
    void strupc(char* str);
    void strskp(char** str);
    void strcrop(char* str);
    void strstrip(char** str);
    int str_has_char(const char* s);

    void* mhs_malloc(int size);
    void* mhs_calloc(int num, int size);
    void mhs_free(void* mem);
    void* mhs_memdup (void const* mem, int size);

    int save_strcmp(const char* s1, const char* s2);
    int save_strcasecmp(const char* s1, const char* s2);

    int find_item(char* s, char* list, char trenner);
    char* find_upc(char* str, char* search);

#ifndef __WIN32__
    unsigned long get_tick(void);
#endif

#ifdef __WIN32__
    void get_unix_time(struct timeval* p);
#else
#define get_unix_time(t) gettimeofday((t), NULL)
#endif

    //void get_timestamp(struct TTime* time_stamp);
    unsigned long mhs_diff_time(unsigned long now, unsigned long stamp);

    char* get_item_as_string(char** str, char* trenner, int* result);
    unsigned long get_item_as_ulong(char** str, char* trenner, int* result);
    long get_item_as_long(char** str, char* trenner, int* result);
    double get_item_as_double(char** str, char* trenner, int* result);

    char* mhs_stpcpy(char* dest, const char* src);
    char* mhs_strdup(const char* str);
    char* mhs_strconcat(const char* string1, ...);
    char* get_file_name(const char* file_name);
    char* create_file_name(const char* dir, const char* file_name);
    char* path_get_dirname(const char* file_name);
    char* change_file_ext(const char* file_name, const char* ext);

#ifdef __cplusplus
}
#endif

#endif
