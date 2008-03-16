/*
 * scr_list.c
 *
 * Defines functions for initializing and displaying any
 * of the list screens.
 * This also includes keyboard actions for the screen.
 *
 */

#include "scr_list.h"

#include "globals.h"
#include "output_scr.h"
#include "input_kb.h"
#include "lev_data.h"
#include "scr_actn.h"
#include "scr_help.h"
#include "scr_thing.h"
#include "obj_things.h"
#include "obj_column.h"
#include "obj_slabs.h"
#include "lev_things.h"
#include "lev_column.h"

// Variables

LIST_DATA *list;

char map_fname[LINEMSG_SIZE];

/*
 * Initializes variables for the list screen.
 */
short init_list(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode)
{
    //Creating and clearing list variable
    list=(LIST_DATA *)malloc(sizeof(LIST_DATA));
    if (list==NULL)
     die("init_list: Cannot allocate memory.");
    return true;
}

/*
 * Deallocates memory for the list screen.
 */
void free_list(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode)
{
  free(list);
}

/*
 * Covers actions from all screens with numbered list.
 */
short actions_list(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    int num_rows=(list->items)/(list->cols)
                        + (((list->items)%(list->cols))>0);
    switch (key)
    {
    case KEY_UP:
      list->pos--;
      break;
    case KEY_DOWN:
      list->pos++;
      break;
    case KEY_LEFT:
      list->pos-=(list->items)/(list->cols)
                        + (((list->items)%(list->cols))>0);
      break;
    case KEY_RIGHT:
      list->pos+=(list->items)/(list->cols)
                        + (((list->items)%(list->cols))>0);
      break;
    case KEY_PGUP:
      list->pos-=scrmode->rows-1;
      break;
    case KEY_PGDOWN:
      list->pos+=scrmode->rows-1;
      break;
    case KEY_HOME:
      list->pos=0;
      break;
    case KEY_END:
      list->pos=list->items-1;
      break;
    default:
      return false;
    }
    if (list->pos >= list->items)
      list->pos=list->items-1;
    if (list->pos<0) list->pos=0;
    //Compute the row where selected item is
    int curr_row=(list->pos)%num_rows;
    if ((list->y)>curr_row) list->y=curr_row;
    if (list->y+scrmode->rows<curr_row+1) list->y=curr_row-scrmode->rows+1;
    if (list->y+scrmode->rows > num_rows+1)
      list->y=num_rows-scrmode->rows+1;
    if (list->y<0) list->y=0;
    message_log(" actions_list: list position is %d",list->pos);
    return true;
}

/*
 * Covers actions from all screens with right panel list.
 */
short actions_rplist(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    int num_rows=(list->items)/(list->cols)
                        + (((list->items)%(list->cols))>0);
    switch (key)
    {
    case KEY_UP:
      list->pos--;
      break;
    case KEY_DOWN:
      list->pos++;
      break;
    case KEY_LEFT:
      list->pos-=(list->items)/(list->cols)
                        + (((list->items)%(list->cols))>0);
      break;
    case KEY_RIGHT:
      list->pos+=(list->items)/(list->cols)
                        + (((list->items)%(list->cols))>0);
      break;
    case KEY_PGUP:
      list->pos-=scrmode->rows-1;
      break;
    case KEY_PGDOWN:
      list->pos+=scrmode->rows-1;
      break;
    case KEY_HOME:
      list->pos=0;
      break;
    case KEY_END:
      list->pos=list->items-1;
      break;
    default:
      return false;
    }
    int scr_rows=scrmode->rows-USRINPUT_ROWS-1;
    if (list->pos >= list->items)
      list->pos=list->items-1;
    if (list->pos<0) list->pos=0;
    //Compute the row where selected item is
    int curr_row=(list->pos)%num_rows;
    if ((list->y)>curr_row) list->y=curr_row;
    if (list->y+scr_rows<curr_row+1) list->y=curr_row-scr_rows+1;
    if (list->y+scr_rows > num_rows+1)
      list->y=num_rows-scr_rows+1;
    if (list->y<0) list->y=0;
    message_log(" actions_rplist: list position is %d",list->pos);
    return true;
}

/*
 * Action function - start any of the the list modes.
 */
short start_list(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int lstmode)
{
    list->prevmode=scrmode->mode;
    list->pos=0; // selected item position (rel. to screen top)
    //These parameters will be reset to proper values on redraw
    list->y=0; //the first visible row
    list->items=8; //number of items in the list
    list->cols=2; //number of columns in the item list
    list->val1=0;
    list->val2=0;
    list->val3=0;
    scrmode->mode=lstmode;
    message_info("Use arrow keys and page up/down to move, "
      "enter to choose.");
    return true;
}

/*
 * Action function - end any of the the list modes.
 */
void end_list(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    if (scrmode->mode!=list->prevmode)
      scrmode->mode=list->prevmode;
    else
      scrmode->mode=MD_SLB;
    message_release();
}

/*
 * Action function - start any of the right panel list modes.
 */
short start_rplist(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int lstmode)
{
    list->prevmode=scrmode->mode;
    list->pos=0; // selected item position (rel. to screen top)
    //These parameters will be reset to proper values on redraw
    list->y=0; //the first visible row
    list->items=8; //number of items in the list
    list->cols=1; //number of columns in the item list
    list->val1=0;
    list->val2=0;
    list->val3=0;
    scrmode->mode=lstmode;
    message_info("Use arrow keys and page up/down to move, "
      "or enter number with keyboard.");
    return true;
}

/*
 * Action function - end any of the the right panel list modes.
 */
void end_rplist(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    if (scrmode->mode!=list->prevmode)
      scrmode->mode=list->prevmode;
    else
      scrmode->mode=MD_SLB;
    message_release();
}

/*
 * Draws screen with a numbered list (as creature types or item types)
 * and key help for given screen.
 */
void draw_numbered_list(char *(*itemstr)(unsigned short),
        struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,
        unsigned int start_idx,unsigned int end_idx,unsigned int itm_width)
{
    unsigned int line_length=min(scrmode->cols,LINEMSG_SIZE);
    unsigned int entry_width=itm_width+5;
    int scr_rows=scrmode->rows;
    list->cols=max((unsigned int)(line_length/entry_width),1);
    list->items=end_idx-start_idx+1;
    unsigned int num_rows=list->items/list->cols
                        + (((list->items)%(list->cols))>0);
    unsigned int i,k;
    int col;
    for (i=0; i < scr_rows; i++)
    {
      set_cursor_pos(i, 0);
      char it_msg[LINEMSG_SIZE];
      if (i+list->y<num_rows)
        for (k=0;k<list->cols;k++)
        {
          int itm_idx=start_idx+list->y+i+k*num_rows;
          if (itm_idx-start_idx==list->pos)
              col=PRINT_COLOR_BLACK_ON_LGREY;
          else
              col=PRINT_COLOR_LGREY_ON_BLACK;
          if (itm_idx<=end_idx)
            sprintf(it_msg,"%3d: %-*.*s",
                    itm_idx,itm_width,itm_width, itemstr(itm_idx));
          else
            sprintf(it_msg,"");
          screen_setcolor(col);
          screen_printf("%s",it_msg);
        }
      screen_setcolor(PRINT_COLOR_LGREY_ON_BLACK);
      screen_printf_toeol("");
      set_cursor_pos(i, scrmode->cols);
      screen_setcolor(PRINT_COLOR_YELLOW_ON_BLUE);
      screen_printchr(' ');
      screen_printchr(' ');
    }
    int scr_row=display_mode_keyhelp(0,scrmode->cols+3,scrmode->mode);
}

/*
 * Draws right panel with a list.
 */
void draw_rpanel_list(char *(*itemstr)(unsigned short),
        struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,
        unsigned int start_idx,unsigned int end_idx,unsigned int itm_width)
{
    unsigned int line_length=min(scrmode->keycols,LINEMSG_SIZE);
    unsigned int entry_width=itm_width+1;
    int scr_rows=scrmode->rows-USRINPUT_ROWS-1;
    list->cols=max((unsigned int)(line_length/entry_width),1);
    list->items=end_idx-start_idx+1;
    unsigned int num_rows=list->items/list->cols
                        + (((list->items)%(list->cols))>0);
    unsigned int i,k;
    int col;
    for (i=0; i < scr_rows; i++)
    {
      set_cursor_pos(i, scrmode->cols+2);
      char it_msg[LINEMSG_SIZE];
      if (i+list->y<num_rows)
        for (k=0;k<list->cols;k++)
        {
          int itm_idx=start_idx+list->y+i+k*num_rows;
          if (itm_idx-start_idx==list->pos)
              col=PRINT_COLOR_BLACK_ON_LGREY;
          else
              col=PRINT_COLOR_LGREY_ON_BLACK;
          if (itm_idx<=end_idx)
            sprintf(it_msg," %-*.*s",
                    itm_width,itm_width, itemstr(itm_idx));
          else
            sprintf(it_msg,"");
          screen_setcolor(col);
          screen_printf("%s",it_msg);
        }
      screen_setcolor(PRINT_COLOR_LGREY_ON_BLACK);
      screen_printf_toeol("");
      set_cursor_pos(i, scrmode->cols);
      screen_setcolor(PRINT_COLOR_YELLOW_ON_BLUE);
      screen_printchr(' ');
      screen_printchr(' ');
    }
}

/*
 * Covers actions from the creature screen.
 */
void actions_crtre(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    int sx, sy;
    sx = (mapmode->map.x+mapmode->screen.x)*3+mapmode->subtl.x;
    sy = (mapmode->map.y+mapmode->screen.y)*3+mapmode->subtl.y;
    unsigned char *thing;
    message_release();
    if (!actions_list(scrmode,mapmode,lvl,key))
    {
      switch (key)
      {
        case KEY_O:
          list->val2=get_owner_next(list->val2);
          break;
        case KEY_S:
        case KEY_SHIFT_S:
          if (list->val1<10)
            list->val1++;
          else
            message_error("Max level reached");
          break;
        case KEY_X:
        case KEY_SHIFT_X:
          if (list->val1>1)
            list->val1--;
          else
            message_error("Min level reached");
          break;
        case KEY_TAB:
        case KEY_DEL:
        case KEY_ESCAPE:
          mdend[MD_CRTR](scrmode,mapmode,lvl);
          message_info("Adding creature cancelled");
          break;
        case KEY_ENTER:
          thing = create_creature(lvl,sx,sy,list->pos+1);
          set_thing_subtile_h(thing,1);
          set_thing_level(thing,list->val1);
          set_thing_owner(thing,list->val2);
          thing_add(lvl,thing);
          // Show the new thing
          mdtng->vistng[mapmode->subtl.x][mapmode->subtl.y]=get_object_subtl_last(lvl,sx,sy,OBJECT_TYPE_THING);
          mdend[MD_CRTR](scrmode,mapmode,lvl);
          message_info("Creature added");
          break;
        default:
          message_info("Unrecognized creature key code: %d",key);
          speaker_beep();
      }
    }
}

/*
 * Covers actions from the item type screen.
 */
void actions_itemt(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    unsigned char *thing;
    int sx=(mapmode->map.x+mapmode->screen.x)*3+mapmode->subtl.x;
    int sy=(mapmode->map.y+mapmode->screen.y)*3+mapmode->subtl.y;
    message_release();
    if (!actions_list(scrmode,mapmode,lvl,key))
    {
      switch (key)
      {
        case KEY_TAB:
        case KEY_DEL:
        case KEY_ESCAPE:
          mdend[MD_ITMT](scrmode,mapmode,lvl);
          message_info("Adding thing cancelled");
          break;
        case KEY_ENTER:
          tng_makeitem(scrmode,mapmode,lvl,sx,sy,list->pos+1);
          mdend[MD_ITMT](scrmode,mapmode,lvl);
          message_info("Item added");
          break;
        default:
          message_info("Unrecognized item key code: %d",key);
          speaker_beep();
      }
    }
}

/*
 * Covers actions from texture selection screen.
 */
void actions_mdtextr(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    message_release();
    if (!actions_list(scrmode,mapmode,lvl,key))
    {
      switch (key)
      {
        case KEY_TAB:
        case KEY_DEL:
        case KEY_ESCAPE:
          mdend[MD_TXTR](scrmode,mapmode,lvl);
          message_info("Texture change cancelled");
          break;
        case KEY_ENTER:
          lvl->inf=list->pos;
          mdend[MD_TXTR](scrmode,mapmode,lvl);
          message_info("Texture changed");
          break;
        default:
          message_info("Unrecognized texture key code: %d",key);
          speaker_beep();
      }
    }
}

/*
 * Covers actions from custom columns screen.
 */
void actions_mdcclm(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    struct DK_CUSTOM_CLM *cclm_recs[9];
    struct COLUMN_REC *clm_recs[9];
    int tx=(mapmode->map.x+mapmode->screen.x);
    int ty=(mapmode->map.y+mapmode->screen.y);
    message_release();
    if (!actions_list(scrmode,mapmode,lvl,key))
    {
      switch (key)
      {
        case KEY_TAB:
        case KEY_DEL:
        case KEY_ESCAPE:
          mdend[MD_CCLM](scrmode,mapmode,lvl);
          message_info("Customization cancelled");
          break;
        case KEY_ENTER:
          {
            int i,k;
            for (k=0;k<3;k++)
              for (i=0;i<3;i++)
              {
                cclm_recs[k*3+i]=create_cust_col();
                clm_recs[k*3+i]=cclm_recs[k*3+i]->rec;
              }
            //Retrieving parameters from LEVEL structure - the slab and its surrounding
            unsigned char *surr_slb=(unsigned char *)malloc(9*sizeof(unsigned char));
            unsigned char *surr_own=(unsigned char *)malloc(9*sizeof(unsigned char));
            unsigned char **surr_tng=(unsigned char **)malloc(9*sizeof(unsigned char *));
            get_slab_surround(surr_slb,surr_own,surr_tng,lvl,tx,ty);
            fill_custom_column_data(list->pos,clm_recs,surr_slb,surr_own,surr_tng);
            for (k=0;k<3;k++)
              for (i=0;i<3;i++)
              {
                cust_col_add_or_update(lvl,tx*3+i,ty*3+k,cclm_recs[k*3+i]);
              }
            mdend[MD_CCLM](scrmode,mapmode,lvl);
            message_info("Custom columns set");
          };break;
        default:
          message_info("Unrecognized custom columns key code: %d",key);
          speaker_beep();
      }
    }
}

/*
 * Covers actions from slab list screen.
 */
void actions_mdslbl(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    message_release();
    if (!actions_list(scrmode,mapmode,lvl,key))
    {
      switch (key)
      {
        case KEY_TAB:
        case KEY_DEL:
        case KEY_ESCAPE:
          mdend[MD_SLBL](scrmode,mapmode,lvl);
          message_info("Slab change cancelled");
          break;
        case KEY_ENTER:
          slb_place_room(lvl,mapmode,list->pos);
          mdend[MD_SLBL](scrmode,mapmode,lvl);
          message_info("New slab placed");
          break;
        default:
          message_info("Unrecognized slab list key code: %d",key);
          speaker_beep();
      }
    }
}

/*
 * Covers actions from object search screen.
 */
void actions_mdsrch(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    message_release();
    if (!actions_list(scrmode,mapmode,lvl,key))
    {
      switch (key)
      {
        case KEY_TAB:
        case KEY_DEL:
        case KEY_ESCAPE:
          mdend[MD_SRCH](scrmode,mapmode,lvl);
          message_info("Object search cancelled");
          break;
        case KEY_ENTER:
          mdend[MD_SRCH](scrmode,mapmode,lvl);
          if (list->pos==0)
          {
            clear_highlight(mapmode);
          }
          int tx=-1;
          int ty=-1;
          int num_found=0;
          unsigned char *obj;
          while (ty<MAP_SIZE_Y)
          {
            obj=find_next_object_on_map(lvl,&tx,&ty,list->pos);
            if (obj!=NULL)
            {
              set_tile_highlight(mapmode,tx,ty,PRINT_COLOR_LRED_ON_YELLOW);
              num_found++;
            } else
            { break; }
          } 
          message_info("Matching objects found: %d",num_found);
          break;
        default:
          message_info("Unrecognized object search key code: %d",key);
          speaker_beep();
      }
    }
}

/*
 * Action function - start texture selection mode.
 */
short start_mdtextr(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_mdtextr: starting");
    short result;
    result=start_list(scrmode,mapmode,lvl,MD_TXTR);
    scrmode->usrinput_type=SI_NONE;
    list->pos=lvl->inf;
    message_log(" start_mdtextr: completed");
    return result;
}

/*
 * Action function - start slab list mode.
 */
short start_mdslbl(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_mdslbl: starting");
    int tx=mapmode->map.x+mapmode->screen.x;
    int ty=mapmode->map.y+mapmode->screen.y;
    short result;
    result=start_list(scrmode,mapmode,lvl,MD_SLBL);
    scrmode->usrinput_type=SI_NONE;
    list->pos=get_tile_slab(lvl,tx,ty);
    message_log(" start_mdslbl: completed");
    return result;
}

void draw_crtre(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    int all_rows=get_screen_rows();
    int all_cols=get_screen_cols();

    draw_numbered_list(get_creature_subtype_fullname,scrmode,mapmode,1,CREATR_SUBTP_FLOAT,16);
    int scr_col1=scrmode->cols+3;
    // Display more info about the creature
    if (all_rows > 7)
    {
      screen_setcolor(PRINT_COLOR_LGREY_ON_BLACK);
      set_cursor_pos(all_rows-10, scr_col1);
      screen_printf("%s","Creature parameters");
      set_cursor_pos(all_rows-8, scr_col1);
      screen_printf("Level: %d",list->val1);
      set_cursor_pos(all_rows-6, scr_col1);
      screen_printf("Owner: %s",get_owner_type_fullname(list->val2));
    }
    set_cursor_pos(get_screen_rows()-1, 17);
}

void draw_itemt(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    draw_numbered_list(get_item_subtype_fullname,scrmode,mapmode,1,ITEM_SUBTYPE_SPELLARMG,18);
    set_cursor_pos(get_screen_rows()-1, 17);
}

void draw_mdtextr(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    draw_numbered_list(get_texture_fullname,scrmode,mapmode,0,INF_MAX_INDEX,55);
    set_cursor_pos(get_screen_rows()-1, 17);
}

void draw_mdcclm(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    draw_numbered_list(get_custom_column_fullname,scrmode,mapmode,0,CUST_CLM_GEN_MAX_INDEX,18);
    set_cursor_pos(get_screen_rows()-1, 17);
}

void draw_mdslbl(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    draw_numbered_list(get_slab_fullname,scrmode,mapmode,0,SLAB_TYPE_GUARDPOST,17);
    set_cursor_pos(get_screen_rows()-1, 17);
}

void draw_mdsrch(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    draw_numbered_list(get_search_objtype_name,scrmode,mapmode,0,get_search_objtype_count()-1,18);
    set_cursor_pos(get_screen_rows()-1, 17);
}

short start_crtre(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_crtre: starting");
    int sx, sy;
    sx = (mapmode->map.x+mapmode->screen.x)*3+mapmode->subtl.x;
    sy = (mapmode->map.y+mapmode->screen.y)*3+mapmode->subtl.y;
    short result;
    result=start_list(scrmode,mapmode,lvl,MD_CRTR);
    scrmode->usrinput_type=SI_NONE;
    // creature level
    list->val1=1;
    // owning player
    list->val2=get_tile_owner(lvl,mapmode->map.x+mapmode->screen.x,mapmode->map.y+mapmode->screen.y);
//    list->pos=
    message_log(" start_crtre: completed");
    return result;
}

void end_crtre(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_list(scrmode,mapmode,lvl);
}

short start_itemt(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_itemt: starting");
    short result;
    result=start_list(scrmode,mapmode,lvl,MD_ITMT);
    scrmode->usrinput_type=SI_NONE;
//    list->pos=
    message_log(" start_itemt: completed");
    return result;
}

void end_itemt(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_list(scrmode,mapmode,lvl);
}

short start_mdcclm(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_mdcclm: starting");
    short result;
    result=start_list(scrmode,mapmode,lvl,MD_CCLM);
    scrmode->usrinput_type=SI_NONE;
//    list->pos=
    message_log(" start_mdcclm: completed");
    return result;
}

void end_mdtextr(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_list(scrmode,mapmode,lvl);
}

void end_mdcclm(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_list(scrmode,mapmode,lvl);
}

void end_mdslbl(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_list(scrmode,mapmode,lvl);
}

short start_mdsrch(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_mdsrch: starting");
    short result;
    result=start_list(scrmode,mapmode,lvl,MD_SRCH);
    scrmode->usrinput_type=SI_NONE;
//    list->pos=
    message_log(" start_mdsrch: completed");
    return result;
}

void end_mdsrch(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_list(scrmode,mapmode,lvl);
}

char *get_listview_map_fname(unsigned short pos_idx)
{
    if (pos_idx<1)
    {
      sprintf (map_fname, "(%s)","user typed");
    } else
    {
      sprintf (map_fname, "map%.5d", pos_idx);
    }
    return map_fname;
}

short start_mdlmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_mdlmap: starting");
    short result;
    result=start_rplist(scrmode,mapmode,lvl,MD_LMAP);
    scrmode->usrinput_type=SI_LDMAP;
    scrmode->usrinput[0]='\0';
    scrmode->usrinput_pos=0;
    list->pos=0;
    level_free(mapmode->preview);
    level_clear(mapmode->preview);
    message_log(" start_mdlmap: completed");
    return result;
}

void end_mdlmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_rplist(scrmode,mapmode,lvl);
    scrmode->usrinput_type=SI_NONE;
}

void draw_mdlmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    struct LEVEL *draw_lvl;
    if ((mapmode->level_preview&LPREV_LOAD) == LPREV_LOAD)
        draw_lvl=mapmode->preview;
    else
        draw_lvl=lvl;
    draw_map_area(scrmode,mapmode,draw_lvl,true,true,false);
    draw_rpanel_list(get_listview_map_fname,scrmode,mapmode,0,9999,12);
    display_rpanel_bottom(scrmode,mapmode,lvl);
}

void actions_mdlmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    message_release();
    short load_preview;
    load_preview=false;
    if (string_get_actions(scrmode,key))
    {
      if (scrmode->usrinput[0]!='\0')
      {
        actions_rplist(scrmode,mapmode,lvl,KEY_HOME);
      }
      load_preview=true;
    } else
    if (actions_rplist(scrmode,mapmode,lvl,key))
    {
      if (list->pos>0)
      {
        char *mapname=get_listview_map_fname(list->pos);
        strncpy(scrmode->usrinput,mapname,LINEMSG_SIZE);
        scrmode->usrinput_pos=strlen(scrmode->usrinput);
      }
      load_preview=true;
    } else
    {
      switch (key)
      {
        case KEY_TAB:
        case KEY_ESCAPE:
          mdend[MD_LMAP](scrmode,mapmode,lvl);
          message_info("Map loading cancelled");
          break;
        case KEY_ENTER:
          {
            popup_show("Loading map","Reading map files. Please wait...");
            short fname_ok=format_map_fname(lvl->fname,scrmode->usrinput);
            mdend[MD_LMAP](scrmode,mapmode,lvl);
            if (!fname_ok)
            {
              message_error("Wrong file name - can't load map");
              break;
            }
            free_map(lvl);
            load_map(lvl);
            clear_highlight(mapmode);
            change_mode(scrmode,mapmode,lvl,scrmode->mode);
            message_info("Map \"%s\" loaded", lvl->fname);
          };break;
        default:
          message_info("Unrecognized \"%s\" key code: %d",longmodenames[MD_LMAP],key);
          speaker_beep();
      }
    }
    if (load_preview)
    {
      format_map_fname(mapmode->preview->fname,scrmode->usrinput);
      if ((mapmode->level_preview&LPREV_LOAD) == LPREV_LOAD)
      {
        if (load_map_preview(mapmode->preview))
          message_info("Map \"%s\" preview loaded",scrmode->usrinput);
      }
    }
}

short start_mdsmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    message_log(" start_mdsmap: starting");
    short result;
    result=start_rplist(scrmode,mapmode,lvl,MD_SMAP);
    scrmode->usrinput_type=SI_SVMAP;
    scrmode->usrinput[0]='\0';
    scrmode->usrinput_pos=0;
    list->pos=0;
    level_free(mapmode->preview);
    level_clear(mapmode->preview);
    message_log(" start_mdsmap: completed");
    return result;
}

void end_mdsmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    end_rplist(scrmode,mapmode,lvl);
    scrmode->usrinput_type=SI_NONE;
}

void draw_mdsmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl)
{
    struct LEVEL *draw_lvl;
    if ((mapmode->level_preview&LPREV_SAVE) == LPREV_SAVE)
        draw_lvl=mapmode->preview;
    else
        draw_lvl=lvl;
    draw_map_area(scrmode,mapmode,draw_lvl,true,true,false);
    draw_rpanel_list(get_listview_map_fname,scrmode,mapmode,0,9999,12);
    display_rpanel_bottom(scrmode,mapmode,lvl);
}

void actions_mdsmap(struct SCRMODE_DATA *scrmode,struct MAPMODE_DATA *mapmode,struct LEVEL *lvl,int key)
{
    message_release();
    short load_preview;
    load_preview=false;
    if (string_get_actions(scrmode,key))
    {
      if (scrmode->usrinput[0]!='\0')
      {
        actions_rplist(scrmode,mapmode,lvl,KEY_HOME);
      }
      load_preview=true;
    } else
    if (actions_rplist(scrmode,mapmode,lvl,key))
    {
      if (list->pos>0)
      {
        char *mapname=get_listview_map_fname(list->pos);
        strncpy(scrmode->usrinput,mapname,LINEMSG_SIZE);
        scrmode->usrinput_pos=strlen(scrmode->usrinput);
      }
      load_preview=true;
    } else
    {
      switch (key)
      {
        case KEY_TAB:
        case KEY_ESCAPE:
          mdend[MD_SMAP](scrmode,mapmode,lvl);
          message_info("Map saving cancelled");
          break;
        case KEY_ENTER:
          {

            popup_show("Saving map","Writing map files. Please wait...");
            short fname_ok=format_map_fname(lvl->savfname,scrmode->usrinput);
            mdend[MD_LMAP](scrmode,mapmode,lvl);
            if (!fname_ok)
            {
              message_error("Map saving cancelled");
              break;
            }
            save_map(lvl);
            message_info("Map \"%s\" saved", lvl->savfname);
          };break;
        default:
          message_info("Unrecognized \"%s\" key code: %d",longmodenames[MD_SMAP],key);
          speaker_beep();
      }
    }
    if (load_preview)
    {
      format_map_fname(mapmode->preview->fname,scrmode->usrinput);
      if ((mapmode->level_preview&LPREV_SAVE) == LPREV_SAVE)
      {
        if (load_map_preview(mapmode->preview))
          message_info("Map \"%s\" preview loaded",scrmode->usrinput);
      }
    }
}
