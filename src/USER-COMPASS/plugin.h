/* -*- c -*- */
/* header for plugin interface */
#ifndef PLUGIN_H
#define PLUGIN_H

#define PLUGIN_ABI 1

#define PLUGIN_OK 1
#define PLUGIN_FAIL 0

#ifdef __cplusplus
extern "C" 
{
#endif

    typedef struct 
    {
        int abi;               /* ABI version of the plugin interface */
        int version;           /* Version of the plugin itself */
        const char *name;      /* Name of the contained class */
        const char *desc;      /* Description of the contained class */
        const char *author;    /* Name of the plugin author */
        void *handle;          /* place to store handle of DSO */

        void *(*create_class)(const char *); /* function pointer for the constructor */
        void (*destroy_class)(void *); /* function pointer for the destructor */
    } plugin_t;

    extern int register_plugin(const char *path, plugin_t *ptr);
    extern void unregister_plugin(plugin_t *ptr);
#ifdef __cplusplus
}
#endif

#endif
