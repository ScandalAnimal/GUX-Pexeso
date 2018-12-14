/*
 * GUX 2 - Pexeso
 * Author: maros vasilisin, xvasil02
 * 14.12.2018
 */
#include <gtk/gtk.h>

#define BOARD_WIDTH 4
#define BOARD_HEIGHT 6

GtkWidget *window;
GdkPixbuf *icon;

GtkWidget *vbox;
GtkWidget *scoreboard;
GtkWidget *menuBar;
GtkWidget *optionsMenu;
GtkWidget *optionsLabel;
GtkWidget *newGameLabel;
GtkWidget *quitLabel;

GtkWidget *table;
GtkWidget *button;
GtkWidget *image;

GtkWidget *whoseTurnitIs;
GtkWidget *player1Text;
GtkWidget *player2Text;
GtkWidget *player1Score;
GtkWidget *player2Score;

int onTurn = 0;
int turns = 0;
int firstMove = 0;
int score1 = 0;
int score2 = 0;

GtkWidget* buttons[BOARD_WIDTH * BOARD_HEIGHT];
int states[BOARD_WIDTH * BOARD_HEIGHT] = {0};
int images[BOARD_WIDTH * BOARD_HEIGHT];

GdkPixbuf *create_pixbuf(const gchar * filename) {
    
   GdkPixbuf *pixbuf;
   GError *error = NULL;
   pixbuf = gdk_pixbuf_new_from_file(filename, &error);
   
   if (!pixbuf) {
       
      fprintf(stderr, "%s\n", error->message);
      g_error_free(error);
   }

   return pixbuf;
}

void initWindow() {

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Superhero Pexeso");
  	gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);
  	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  	gtk_container_set_border_width(GTK_CONTAINER(window), 5);

	icon = create_pixbuf("back.png");  
  	gtk_window_set_icon(GTK_WINDOW(window), icon);
}

void initMenu() {

    vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), vbox);

    menuBar = gtk_menu_bar_new();
  	optionsMenu = gtk_menu_new();

  	optionsLabel = gtk_menu_item_new_with_label("Options");
  	newGameLabel = gtk_menu_item_new_with_label("New Game");
  	quitLabel = gtk_menu_item_new_with_label("Quit");

    gtk_menu_item_set_submenu(GTK_MENU_ITEM(optionsLabel), optionsMenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu), newGameLabel);
    gtk_menu_shell_append(GTK_MENU_SHELL(optionsMenu), quitLabel);
    gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), optionsLabel);
    gtk_box_pack_start(GTK_BOX(vbox), menuBar, FALSE, FALSE, 0);

}

char* getImageName(int position) {

	int imageNumber = images[position];
	char str[12];
	sprintf(str, "%d", imageNumber);
	
	char* res = (char*)malloc(strlen(str) + strlen(".png") + 1);
	strcpy(res, str);
	strcat(res, ".png");
	return res;

}

void shuffleImages() {
	
	int size = BOARD_HEIGHT * BOARD_WIDTH;
	if (size > 1) {
        size_t i;
        for (i = 0; i < size - 1; i++) {
          size_t j = i + rand() / (RAND_MAX / (size - i) + 1);
          int t = images[j];
          images[j] = images[i];
          images[i] = t;
        }
    }
}


void newGame( GtkWidget *widget, gpointer data) {

	for (int i = 0; i < (BOARD_HEIGHT * BOARD_WIDTH); i++) {
		image = gtk_image_new_from_file("back.png");
		gtk_button_set_image (GTK_BUTTON (buttons[i]), image);
	}
	int i = 0;
	char str[12];
	sprintf(str, "%d", i);
	gtk_label_set_text(GTK_LABEL(player1Score), str);
	gtk_label_set_text(GTK_LABEL(player2Score), str);
	gtk_label_set_text(GTK_LABEL(whoseTurnitIs), "Turn: Player 1");
	score1 = 0;
	score2 = 0;
	for (int i = 0; i < (BOARD_HEIGHT * BOARD_WIDTH); i++) {
		states[i] = 0;
	}
	// shuffleImages();
 }

void showScore() {
	
	GtkWidget *dialog;
	char str[12];
	sprintf(str, "%d", score1);
	
	char* res = (char*)malloc(strlen("Final score is: ") + strlen(str) + strlen(":") + strlen(str) + strlen(".\nDo you want to play another game?")+ 1);
	strcpy(res, "Final score is: ");
	strcat(res, str);
	sprintf(str, "%d", score2);
	strcat(res, ":");
	strcat(res, str);
	strcat(res, ".\nDo you want to play another game?");

	dialog = gtk_message_dialog_new(GTK_WINDOW(window),
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_QUESTION,
            GTK_BUTTONS_OK_CANCEL,
            res);

  	gtk_window_set_title(GTK_WINDOW(dialog), "Final Score");
  	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	switch (result) {
	    case GTK_RESPONSE_OK:
    	    newGame(dialog, NULL);
       		break;
    	default:
       		gtk_main_quit();
       		break;
  	}
	gtk_widget_destroy(dialog);
	free(res);

}

void turn( GtkWidget *widget, gpointer data) {
	char* imageName = getImageName(GPOINTER_TO_INT(data)); 
	GtkWidget *image2 = gtk_image_new_from_file(imageName);
	gtk_button_set_image (GTK_BUTTON (widget), image2);

    turns++;
	gtk_widget_show_all(window);
    free(imageName);

    if (states[GPOINTER_TO_INT(data)] == 0) { 
	    if (turns == 1) {
	    	firstMove = GPOINTER_TO_INT(data);
	    }
	    if (turns == 2) {

	    	if (firstMove != GPOINTER_TO_INT(data)) {
		    	int a = images[firstMove];
		    	int b = images[GPOINTER_TO_INT(data)];
		    	if (a == b) {
		    		states[firstMove] = 1;
		    		states[GPOINTER_TO_INT(data)] = 1;
		    		if (onTurn == 1) {
		    			score1++;
		    			char str[12];
						sprintf(str, "%d", score1);
						gtk_label_set_text(GTK_LABEL(player1Score), str);
						if ((score1 + score2) == (BOARD_WIDTH * BOARD_HEIGHT / 2)) {
							showScore();
						}
					}
					else {
		    			score2++;
		    			char str[12];
						sprintf(str, "%d", score2);
						gtk_label_set_text(GTK_LABEL(player2Score), str);
						if ((score1 + score2) == (BOARD_WIDTH * BOARD_HEIGHT / 2)) {
							showScore();
						}
					}

		    	}
		    	else {
					GtkWidget *image3;
					GtkWidget *image4;

					image3 = gtk_image_new_from_file("back.png");
					image4 = gtk_image_new_from_file("back.png");
					gtk_button_set_image (GTK_BUTTON (buttons[firstMove]), image3);
					gtk_button_set_image (GTK_BUTTON (buttons[GPOINTER_TO_INT(data)]), image4);
					if (onTurn == 1) {
						onTurn = 2;
						gtk_label_set_text(GTK_LABEL(whoseTurnitIs), "Turn: Player 2");
					}
					else {
						onTurn = 1;
						gtk_label_set_text(GTK_LABEL(whoseTurnitIs), "Turn: Player 1");
					}
		    	}
		    	firstMove = 0;
		    	turns = 0;
		    }
		    else {
		    	turns = 1;
		    }
	    }
	}
	else {
		turns -= 1;
	}
}

void createImages() {

	int it = 1;
	for (int i = 0; i < (BOARD_WIDTH * BOARD_HEIGHT); i = i + 2) {
		images[i] = it; 
		images[i+1] = it;
		it++; 
	}

	// shuffleImages();
}

void initScoreboard() {
	
	whoseTurnitIs = gtk_label_new("Turn: Player 1");
	onTurn = 1;
	player1Text = gtk_label_new("Player 1 score: ");
	player2Text = gtk_label_new("Player 2 score: ");
	player1Score = gtk_label_new("0");
	player2Score = gtk_label_new("0");

  	scoreboard = gtk_hbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(vbox), scoreboard);

    gtk_box_pack_start(GTK_BOX(scoreboard), player1Text, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(scoreboard), player1Score, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(scoreboard), whoseTurnitIs, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(scoreboard), player2Text, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(scoreboard), player2Score, TRUE, TRUE, 0);

}

void initBoard() {

	table = gtk_table_new(BOARD_WIDTH, BOARD_HEIGHT, TRUE);
    gtk_table_set_row_spacings(GTK_TABLE(table), 2);
    gtk_table_set_col_spacings(GTK_TABLE(table), 2);

    createImages();

    int i = 0;
    int j = 0;
    int pos = 0;

    for (i=0; i < BOARD_WIDTH; i++) {
        for (j=0; j < BOARD_HEIGHT; j++) {
        	image = gtk_image_new_from_file("back.png");
        	button = gtk_button_new();
        	gtk_button_set_image (GTK_BUTTON (button), image);
        	buttons[pos] = button;

			g_signal_connect(button, "clicked", G_CALLBACK(turn), GINT_TO_POINTER(pos));
      		gtk_table_attach_defaults(GTK_TABLE(table), button, j, j+1, i, i+1);
			gtk_widget_show (button);
      		pos++;
    	}
  	}
  	gtk_container_add(GTK_CONTAINER(vbox), table);

}

int main(int argc, char *argv[]) {

	gtk_init(&argc, &argv);

	initWindow();

  	initMenu();
	
	initScoreboard();
	initBoard();

	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(newGameLabel), "activate", G_CALLBACK(newGame), NULL);
	g_signal_connect(G_OBJECT(quitLabel), "activate", G_CALLBACK(gtk_main_quit), NULL);

	gtk_widget_show_all(window);

	g_object_unref(icon);    

	gtk_main();

	return 0;
}