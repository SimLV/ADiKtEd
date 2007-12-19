/*
 * lev_column.h
 *
 * Header file for lev_column.c.
 *
 */

#ifndef ADIKT_LEVCOLMN_H
#define ADIKT_LEVCOLMN_H

struct LEVEL;
struct COLUMN_REC;

void set_clm_ent_idx(struct LEVEL *lvl, int num, unsigned int use, int permanent,
        int lintel, int height, unsigned int solid, int base, int orientation,
        int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7);
void set_clm(struct LEVEL *lvl, int num, unsigned int use, int base,
        int c0, int c1, int c2, int c3, int c4, int c5, int c6, int c7);

int column_find_or_create(struct LEVEL *lvl,struct COLUMN_REC *clm_rec);
int column_get_free_index(struct LEVEL *lvl);
short columns_verify(struct LEVEL *lvl, char *err_msg);

unsigned int get_dat_subtile(struct LEVEL *lvl, unsigned int sx, unsigned int sy);
void set_dat_subtile(struct LEVEL *lvl, int sx, int sy, int d);
void set_dat (struct LEVEL *lvl,int x, int y, int tl, int tm, int tr,
        int ml, int mm, int mr, int bl, int bm, int br);
void set_dat_unif (struct LEVEL *lvl, int x, int y, int d);
short dat_verify(struct LEVEL *lvl, char *err_msg);

void update_datclm_for_whole_map(struct LEVEL *lvl);
void update_datclm_for_square_radius1(struct LEVEL *lvl, int tx, int ty);
void update_datclm_for_square(struct LEVEL *lvl, int tx_first, int tx_last,
    int ty_first, int ty_last);
void update_datclm_for_slab(struct LEVEL *lvl, int tx, int ty);
void set_new_datclm_values(struct LEVEL *lvl, int tx, int ty, struct COLUMN_REC *clm_recs[9]);
void update_clm_utilize_counters(struct LEVEL *lvl);
void get_slab_surround(unsigned char *surr_slb,unsigned char *surr_own,unsigned char **surr_tng,int x, int y);
void clm_utilize_dec(struct LEVEL *lvl, int clmidx);
void clm_utilize_inc(struct LEVEL *lvl, int clmidx);
short clm_entry_is_used(unsigned int clmidx);

void update_tile_wib_entries(struct LEVEL *lvl, int tx, int ty);

unsigned char *get_subtile_column(struct LEVEL *lvl, int sx, int sy);
short get_subtile_column_rec(struct LEVEL *lvl, struct COLUMN_REC *clm_rec, int sx, int sy);
unsigned short get_subtile_column_height(struct LEVEL *lvl, int sx, int sy);
unsigned short get_subtile_column_solid(struct LEVEL *lvl, int sx, int sy);

// This one is special - put in separate file
void add_permanent_columns(struct LEVEL *lvl);

#endif // ADIKT_LEVCOLMN_H