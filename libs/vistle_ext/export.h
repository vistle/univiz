#ifndef VISTLE_UNIVIZ_EXPORT_H
#define VISTLE_UNIVIZ_EXPORT_H

#include <vistle/util/export.h>

#if defined (vistle_univiz_EXPORTS)
#define V_UNIVIZEXPORT V_EXPORT
#else
#define V_UNIVIZEXPORT V_IMPORT
#endif

#endif
