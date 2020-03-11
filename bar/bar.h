/* GTK-based button bar
 * Copyright (c) 2020 Piotr Miller
 * e-mail: nwg.piotr@gmail.com
 * Website: http://nwg.pl
 * Project: https://github.com/nwg-piotr/nwg-launchers
 * License: GPL3
 * */
 
#include <iostream>
#include <fstream>
#include <filesystem>
#include <gtkmm.h>
#include "nlohmann/json.hpp"	// nlohmann-json package
#include <glibmm/ustring.h>
#include <sys/stat.h>

#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>

namespace fs = std::filesystem;
namespace ns = nlohmann;

int image_size (72);			// button image size in pixels
double opacity (0.9);			// overlay window opacity
std::string wm;					// detected window manager name
std::string definition_file {"bar.json"};
std::string orientation {"h"};
std::string h_align {""};
std::string v_align {""};

#ifndef CGTK_APP_BOX_H
#define CGTK_APP_BOX_H
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
#endif // CGTK_APP_BOX_H

#ifndef CGTK_MAIN_WINDOW_H
#define CGTK_MAIN_WINDOW_H
	class MainWindow : public Gtk::Window {
		public:
			MainWindow();
			void set_visual(Glib::RefPtr<Gdk::Visual> visual);
			virtual ~MainWindow();

			Gtk::Grid favs_grid;					// Favourites grid above
			Gtk::Separator separator;				// between favs and all apps
			std::list<AppBox*> all_boxes {};		// attached to apps_grid unfiltered view
			std::list<AppBox*> filtered_boxes {};	// attached to apps_grid filtered view
			std::list<AppBox*> boxes {};		// attached to favs_grid

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
#endif // CGTK_MAIN_WINDOW_H

#ifndef CGTK_DESKTOP_ENTRY_H
#define CGTK_DESKTOP_ENTRY_H
	struct DesktopEntry {
		std::string name;
		std::string exec;
		std::string icon;
		std::string comment;
	};
#endif // CGTK_DESKTOP_ENTRY_H

#ifndef CGTK_CACHE_ENTRY_H
#define CGTK_CACHE_ENTRY_H
		struct BarEntry {
			std::string name;
			std::string exec;
			std::string icon;
		};
#endif // CGTK_CACHE_ENTRY_H