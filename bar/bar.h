/* GTK-based button bar
 * Copyright (c) 2020 Piotr Miller
 * e-mail: nwg.piotr@gmail.com
 * Website: http://nwg.pl
 * Project: https://github.com/nwg-piotr/nwg-launchers
 * License: GPL3
 * */

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>

#include <iostream>
#include <fstream>
#include <filesystem>

#include <gtkmm.h>
#include <glibmm/ustring.h>

#include <nlohmann/json.hpp>    // nlohmann-json package

#include "nwgconfig.h"

namespace fs = std::filesystem;
namespace ns = nlohmann;

extern int image_size;
extern double opacity;
extern std::string wm;

class AppBox : public Gtk::Button {
    public:
        AppBox();
        Glib::ustring name;
        Glib::ustring exec;
        Glib::ustring comment;

        AppBox(Glib::ustring name, std::string exec, Glib::ustring comment) {
            this -> name = name;
            Glib::ustring n = this -> name;
            if (n.length() > 25) {
                n = this -> name.substr(0, 22) + "...";
            }
            this -> exec = exec;
            this -> comment = comment;
            this -> set_always_show_image(true);
            this -> set_label(n);
        }

        virtual ~AppBox();

};

class MainWindow : public Gtk::Window {
    public:
        MainWindow();
        void set_visual(Glib::RefPtr<Gdk::Visual> visual);
        virtual ~MainWindow();

        Gtk::Grid favs_grid;                    // Favourites grid above
        Gtk::Separator separator;               // between favs and all apps
        std::list<AppBox*> all_boxes {};        // attached to apps_grid unfiltered view
        std::list<AppBox*> filtered_boxes {};   // attached to apps_grid filtered view
        std::list<AppBox*> boxes {};            // attached to favs_grid

    private:
        //Override default signal handler:
        bool on_key_press_event(GdkEventKey* event) override;
        void filter_view();
        void rebuild_grid(bool filtered);

    protected:
        virtual bool on_draw(const ::Cairo::RefPtr< ::Cairo::Context>& cr);
        void on_screen_changed(const Glib::RefPtr<Gdk::Screen>& previous_screen);
        bool _SUPPORTS_ALPHA = false;
};

struct DesktopEntry {
    std::string name;
    std::string exec;
    std::string icon;
    std::string comment;
};

struct BarEntry {
    std::string name;
    std::string exec;
    std::string icon;
};

/*
 * Function declarations
 * */
ns::json string_to_json(std::string);
ns::json get_bar_json(std::string);
std::vector<BarEntry> get_bar_entries(ns::json);

std::vector<int> display_geometry(std::string, MainWindow &);
Gtk::Image* app_image(std::string);

void on_button_clicked(std::string);
