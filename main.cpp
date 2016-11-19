#include <gtk/gtk.h>
#include "iostream"

using namespace std;

struct ResizeData {
    gint lastX;
    gint lastY;
    gint lastWidth;
    gint lastHeight;
};

struct InitialData {
    gint x;
    gint y;
    gint width;
    gint height;
};

static void configure_event(GtkWindow *window, GdkEvent *event, ResizeData *data) {
    gint x, y, width, height;
    gtk_window_get_position(window, &x, &y);
    gtk_window_get_size(window, &width, &height);

    if (data->lastX == x && data->lastY == y && data->lastWidth == width && data->lastHeight == height) {
        return;
    }

    cerr << "move:" << x << ":" << y << ":" << width << ":" << height << "\n";
//    printf("move:%d:%d:%d:%d\n", x, y, width, height);

    data->lastX = x;
    data->lastY = y;
    data->lastWidth = width;
    data->lastHeight = height;
}

static void lose_focus(GtkWindow *window, gpointer data) {
    cerr << "blur\n";
}

static void mouse_down(GtkWindow *window, GdkEvent *event) {
    gtk_window_begin_move_drag(window, event->button.button, (gint) event->button.x_root, (gint) event->button.y_root,
                               event->button.time);
    gtk_window_set_keep_above(GTK_WINDOW(window), true);
}

static void activate(GtkApplication *app, InitialData *data) {
    if (!gdk_screen_is_composited(gdk_screen_get_default())) {
        cerr << "Screen does not support compositing.\n";
        _exit(1);
    }

    GtkWidget *window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW (window), "Area Selector");
    gtk_window_set_default_size(GTK_WINDOW (window), data->width, data->height);
    gtk_window_move(GTK_WINDOW(window), data->x, data->y);
//    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_set_decorated(GTK_WINDOW(window), false);
    gtk_window_set_keep_above(GTK_WINDOW(window), true);

    gtk_widget_set_events(GTK_WIDGET(window), GDK_FOCUS_CHANGE_MASK);
    gtk_widget_set_visual(GTK_WIDGET(window), gdk_screen_get_rgba_visual(gdk_screen_get_default()));

    ResizeData *resizeData = new ResizeData();
    gtk_widget_add_events(GTK_WIDGET(window), GDK_STRUCTURE_MASK);
    g_signal_connect(window, "configure-event", G_CALLBACK(configure_event), resizeData);

    g_signal_connect(window, "button-press-event", G_CALLBACK(mouse_down), NULL);
    g_signal_connect(window, "focus-out-event", G_CALLBACK(lose_focus), NULL);

    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    gtk_css_provider_load_from_path(provider, "styles.css", &error);
    if (error != NULL) {
        printf("Error %s\n", error->message);
        return;
    }
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 2);
    gtk_container_add(GTK_CONTAINER(window), box);

    gtk_widget_show_all(window);
}

int create_window(InitialData *data) {

    GtkApplication *app;
    int status;

    app = gtk_application_new("net.burngames.linuxareaselector", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(activate), data);
    status = g_application_run(G_APPLICATION (app), 0, NULL);
    g_object_unref(app);

    return status;
}

int main(int argc, char *argv[]) {
    if (argc != 5) {
        cerr << "Invalid usage: LinuxAreaSelector <x> <y> <width> <height>\n";
        return 1;
    }

    InitialData *data = new InitialData();
    data->x = atoi(argv[1]);
    data->y = atoi(argv[2]);
    data->width = atoi(argv[3]);
    data->height = atoi(argv[4]);

    return create_window(data);
}