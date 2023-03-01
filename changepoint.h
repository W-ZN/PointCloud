#ifndef CHANGEPOINT_H
#define CHANGEPOINT_H

#include <ogr_spatialref.h>

struct Converter
{
    OGRCoordinateTransformation *poCT;
    Converter() : poCT{nullptr} {}
    ~Converter() {
        if (poCT) {
            delete poCT;
            poCT = nullptr;
        }
    }
    void init_epsg(int insrs, const char *gdal_path, const char *proj_path) {
        CPLSetConfigOption("GDAL_DATA", gdal_path);
        const char *proj_search_paths[2] = {proj_path, nullptr};
        OSRSetPROJSearchPaths(proj_search_paths);
        OGRSpatialReference inRs, outRs;
        inRs.importFromEPSG(insrs);
        outRs.importFromEPSG(4326);
        outRs.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        poCT = OGRCreateCoordinateTransformation(&inRs, &outRs);
    }
    void init_wkt(const char *wkt, const char *gdal_path, const char *proj_path) {
        CPLSetConfigOption("GDAL_DATA", gdal_path);
        const char *proj_search_paths[2] = {proj_path, nullptr};
        OSRSetPROJSearchPaths(proj_search_paths);
        OGRSpatialReference inRs, outRs;
        inRs.importFromWkt(&wkt);
        outRs.importFromEPSG(4326);
        outRs.SetAxisMappingStrategy(OAMS_TRADITIONAL_GIS_ORDER);
        poCT = OGRCreateCoordinateTransformation(&inRs, &outRs);
    }
    bool convert(double *val) {
        if (poCT && poCT->Transform(1, val, val + 1)) {
            return true;
        }
        return false;
    }
};


#endif // CHANGEPOINT_H
