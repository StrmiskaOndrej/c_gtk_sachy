#include <gtk/gtk.h>
#include <stdio.h>
#include "int2utf8.h"
#include <gdk/gdkkeysyms.h>

#ifndef GDK_KEY_n
#define GDK_KEY_n GDK_n
#define GDK_KEY_o GDK_o
#define GDK_KEY_s GDK_s
#define GDK_KEY_x GDK_x
#define GDK_KEY_a GDK_a
#define GDK_KEY_v GDK_v
#endif

void destroy_signal(GtkWidget *widget, gpointer data);
gboolean delete_event(GtkWidget *widget, GdkEvent *event, gpointer data);

void zacitNovouHru(GtkWidget *widget, gpointer data);

static gboolean button_pressed (GtkWidget*, GdkEventButton*, GtkLabel *[][8]);
static const GdkRGBA zelena  = {0.5899, 0.8867, 0.3906, 1};
static const GdkRGBA tmaveHneda = {0.8242, 0.5508, 0.2773, 1};
static const GdkRGBA svetleHneda = {0.9805, 0.8047, 0.6094, 1};
GdkColor predchoziBarva;
int kliknuto = 0; //pomocná proměnná sloužící pro kliknutí myší
int hrac = 0; //proměnná zjišťující zda je hráč na tahu
GtkWidget *table, *prevEventbox, *hpanel, *infogrid, *textview, *scroll_win;
GtkTextBuffer *buffer;
GtkTextIter txtiter;
GtkLabel *currenthrac, *row_label, *col_label;
GtkLabel *labelBoard[8][8];
int poziceXInt = 0;
int poziceYInt = 0;
char poziceY[10];
char poziceX[10];

char mnum[11]; // počet odehraných kol
int tah[4]; //src row col; dest row col 
int plocha[8][8];
int movscnt = 0;
int konecHry;

extern int provedTah(int hrac, int *tah, int plocha[][8]);
extern void nastavPlochu(int tmpBoard[][8]);
extern void resetujPassantPole(void);

int main (int argc, char *argv[])
{
	GtkWidget *vbox, *menubar, *soubor, *souborMenu, *b_novaHra;
	GtkAccelGroup *accel_group;

	nastavPlochu(plocha);
	resetujPassantPole();
	GtkWidget *window, *eventbox;
    GtkLabel *label;
	gtk_init (&argc, &argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW (window), "Chess plocha");
  	gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	gtk_widget_set_size_request(window, 680,350);


#if GTK_MAJOR_VERSION >= 3
	vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
#endif
		gtk_container_add(GTK_CONTAINER(window), vbox);
		menubar = gtk_menu_bar_new();
	table = gtk_grid_new ();

	char *figurky[64] = { "♜", "♞", "♝","♛","♚","♝","♞","♜", // výchozí textový obsah
						 "♟", "♟", "♟","♟","♟","♟","♟","♟",
						 "", "", "", "", "", "", "", "",
						 "", "", "", "", "", "", "", "",
						 "", "", "", "", "", "", "", "",
						 "", "", "", "", "", "", "", "",
						 "♙", "♙", "♙","♙","♙","♙","♙","♙",
						 "♖", "♘", "♗","♕","♔","♗","♘","♖"};
	int i,j;
	int p = 0;
	int oddCol = 1;
	for (i = 0; i < 8; i ++) { // Nastavuje všechna pole
		for (j = 0; j < 8; j++) {
			label = (GtkLabel *) gtk_label_new(figurky[p]);
      gtk_widget_set_size_request((GtkWidget *) label, 56, 56); // velikost polí
			labelBoard[i][j]=label;
			eventbox = gtk_event_box_new();
				if (oddCol) { // nastavuje barvu jestnolivých polí
					gtk_widget_override_background_color(eventbox, GTK_STATE_NORMAL, &svetleHneda);
					oddCol = 0;
				} else {
					gtk_widget_override_background_color(eventbox, GTK_STATE_NORMAL, &tmaveHneda);
					oddCol = 1;
				}

			gtk_event_box_set_above_child(GTK_EVENT_BOX(eventbox),FALSE);
			gtk_widget_override_font((GtkWidget *) label, pango_font_description_from_string(
									"Serif 26"));
			gtk_container_add(GTK_CONTAINER (eventbox), (GtkWidget *) label);
			 gtk_grid_attach((GtkGrid *) table, eventbox,j+1,i,1,1);


			g_signal_connect(G_OBJECT (eventbox), "button_press_event",
					G_CALLBACK (button_pressed), (gpointer) labelBoard);

			gtk_widget_set_events(eventbox, GDK_BUTTON_PRESS_MASK);

			p++;
		}
			if(oddCol){ // při změně řádku barva pole zůstává
				oddCol = 0;
			}else{
				oddCol = 1;
			}

	}

  sprintf(mnum,"%s","`");
  for (i = 1; i < 9; i++) { //Nstavení spodního panelu
    mnum[0]++;
	  label = (GtkLabel *) gtk_label_new(mnum);
    gtk_widget_set_size_request((GtkWidget *) label, 0, 30);
    gtk_grid_attach((GtkGrid *) table,(GtkWidget *) label,i,9,1,1);
  }
  i = 8;
  for (j = 0; j < 8; j++) { //Nsatvení postraního panelu
    sprintf(mnum,"%d",i--);
	  label = (GtkLabel *) gtk_label_new(mnum);
    gtk_widget_set_size_request((GtkWidget *) label, 30, 0);
    gtk_grid_attach((GtkGrid *) table,(GtkWidget *) label,0,j,1,1);
  }

	hpanel = gtk_grid_new();
  gtk_grid_attach((GtkGrid *) hpanel, table, 0, 0, 1,1);


  infogrid = gtk_grid_new();
  textview = gtk_text_view_new();
  gtk_text_view_set_editable(GTK_TEXT_VIEW(textview), FALSE);
  gtk_text_view_set_cursor_visible (GTK_TEXT_VIEW (textview), FALSE);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (textview), GTK_WRAP_WORD);
  buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (textview));
  gtk_text_view_set_left_margin (GTK_TEXT_VIEW (textview), 10);
  gtk_text_view_set_right_margin (GTK_TEXT_VIEW (textview), 10);

	/*nastaví informační panel vedle hlavní plochy.*/
  scroll_win = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_set_size_request(scroll_win, 250,431);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scroll_win),
  GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
  gtk_container_add(GTK_CONTAINER (scroll_win), textview);
  gtk_grid_attach((GtkGrid *) infogrid, (GtkWidget *) scroll_win,0,1,1,1);
	currenthrac = (GtkLabel *) gtk_label_new("Aktuální hráč: Bílý");
  gtk_grid_attach((GtkGrid *) infogrid, (GtkWidget *) currenthrac,0,0,1,1);
  gtk_grid_attach((GtkGrid *) hpanel, (GtkWidget *) infogrid, 1,0,1,1);

	gtk_box_pack_start(GTK_BOX(vbox), menubar, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hpanel, TRUE, TRUE, 0);



	accel_group = gtk_accel_group_new();
	gtk_window_add_accel_group(GTK_WINDOW(window), accel_group);

	/* Vysouvací menu */
	soubor = gtk_menu_item_new_with_mnemonic("_Menu");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), soubor);
	souborMenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(soubor), souborMenu);


b_novaHra = gtk_menu_item_new_with_mnemonic("_Nová hra");
	gtk_widget_add_accelerator(b_novaHra, "activate", accel_group,
	GDK_KEY_n, GDK_CONTROL_MASK, GTK_ACCEL_VISIBLE);
	gtk_menu_shell_append(GTK_MENU_SHELL(souborMenu), b_novaHra);
	g_signal_connect(G_OBJECT(b_novaHra), "activate", G_CALLBACK(zacitNovouHru), NULL); 


	gtk_widget_show_all (window);
	g_signal_connect_swapped(G_OBJECT(window), "destroy",
		G_CALLBACK(gtk_main_quit), NULL);
	gtk_main();
	return 0;
}

/*Vykreslí plochu*/
int vykresliPlochu(GtkLabel *labels[][8], int cliBoard[][8])
{
		int i, j;
		for (i = 0; i < 8; i++) {
			for (j = 0; j < 8; j++) {
				gtk_label_set_text(labels[i][j], convertClipieceToGuiPiece(cliBoard[i][j]));
			}
		}
		return 0;
}

/*pojmenuje v horizontální liště hodnoty na a až h */
void prevedY(){
	if(poziceYInt == 1){
		poziceY[0] = 'a';
	}else if(poziceYInt == 2){
		poziceY[0] = 'b';
	}else if(poziceYInt == 3){
		poziceY[0] = 'c';
	}else if(poziceYInt == 4){
		poziceY[0] = 'd';
	}else if(poziceYInt == 5){
		poziceY[0] = 'e';
	}else if(poziceYInt == 6){
		poziceY[0] = 'f';
	}else if(poziceYInt == 7){
		poziceY[0] = 'g';
	}else if(poziceYInt == 8){
		poziceY[0] = 'h';
	}
}

/* Funkce zavolající se, při stisknutí tlačítka myši*/
static gboolean button_pressed (GtkWidget *ebox, GdkEventButton *event,
			GtkLabel *labelBoard[][8])
{
	unsigned left, top, width, height;
	if(!konecHry){
		if (event->type == GDK_BUTTON_PRESS)
		{
			if (!kliknuto) {
				gtk_widget_override_background_color(ebox, GTK_STATE_NORMAL, &zelena);
				gtk_container_child_get(GTK_CONTAINER(table), ebox,
						"left-attach", &left,
						"top-attach",&top,
						"width",&width,
						"height",&height,
						NULL);
		  //uloží x a y pozici ze které je tah prováděn
		  tah[0] = left-1; //Výchozí Y pozice z Leva
		  tah[1] = top;	//Výchozí X pozice z Hora
				/*save label*/
				prevEventbox = ebox;
				kliknuto = 1;
			} else {
				gtk_container_child_get(GTK_CONTAINER(table), ebox,
						"left-attach",  &left,
						"top-attach",   &top,
						"width", &width,
						"height",&height,
						NULL);
		  //uloží x a y pozici do které je tah prováděn
		  tah[2] = left-1; //Cílová Y pozice z Leva
		  tah[3] = top;	//Cílová X pozice z Hora
		  poziceXInt = 8 - top;
		  poziceYInt = left;
				/*změní barvu ze zelené zpátky na hnědou*/
				if ((tah[0]+tah[1])&1){
					gtk_widget_override_background_color(prevEventbox, GTK_STATE_NORMAL, &tmaveHneda);
				} else {
					gtk_widget_override_background_color(prevEventbox, GTK_STATE_NORMAL, &svetleHneda);
				}
		  prevedY();
				int u = provedTah(hrac, tah, plocha);
				if (!u) { 
			buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW (textview));
			gtk_text_buffer_get_end_iter(buffer, &txtiter);
			if (!hrac) {
			  sprintf(mnum,"%d",++movscnt);
			  gtk_text_buffer_insert(buffer, &txtiter, mnum, -1);
			  gtk_text_buffer_insert(buffer, &txtiter, ".", -1);
			}
			/* Updatuje GUI */
					vykresliPlochu(labelBoard, plocha);	
			/* Updatuje herní info */


			sprintf(poziceX, "%d", poziceXInt);
			gtk_text_buffer_insert(buffer, &txtiter, poziceY, -1);
			gtk_text_buffer_insert(buffer, &txtiter, poziceX, -1);
			gtk_text_buffer_insert(buffer, &txtiter, "     ", -1);

			hrac = !hrac; 
			if (hrac) {
				gtk_label_set_text(currenthrac, "Aktuální hráč: Černý");
			} else {
				gtk_label_set_text(currenthrac, "Aktuální hráč: Bílý");
				gtk_text_buffer_insert(buffer, &txtiter, "\n", -1);
			}
			if(konecHry){
				gtk_label_set_text(currenthrac, "Konec hry");
				gtk_text_buffer_insert(buffer, &txtiter, "\n", -1);
				gtk_text_buffer_insert(buffer, &txtiter, "Hra je ukončena", -1);
				gtk_text_buffer_insert(buffer, &txtiter, "\n", -1);
				if(konecHry == 1){
					gtk_text_buffer_insert(buffer, &txtiter, "Vyhrál bílý hráč", -1);
				}else{
					gtk_text_buffer_insert(buffer, &txtiter, "Vyhrál černý hráč", -1);
				}
			}
				}
				kliknuto = 0;
			}
		}

	}
	return FALSE;
	
}

/*funkce zjišťující zda je hra ukočena*/
int jeKonec(int vyherce){
	if(vyherce == 1)
	{
		konecHry = 1;
	}else{
		konecHry = 2;
	}	
}

/* pustí novou hru */
void zacitNovouHru(GtkWidget *widget, gpointer data)
{

	konecHry = 0;
	hrac = 0;
	nastavPlochu(plocha);
	resetujPassantPole();
	vykresliPlochu(labelBoard, plocha);	
	gtk_text_buffer_insert(buffer, &txtiter, "\n", -1);
	gtk_text_buffer_insert(buffer, &txtiter, "Start nové hry", -1);
	gtk_text_buffer_insert(buffer, &txtiter, "\n", -1);
}