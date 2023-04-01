#include "imageapp.h"
#include "mainwindow.h"
#include <iostream>

ImageApp::ImageApp()
: Gtk::Application()
{
  Glib::set_application_name("Image processing");
}

Glib::RefPtr<ImageApp> ImageApp::create()
{
  return Glib::make_refptr_for_instance<ImageApp>(new ImageApp());
}

void ImageApp::on_startup()
{
  //Call the base class's implementation:
  Gtk::Application::on_startup();

  //Create actions for menus and toolbars.
  //We can use add_action() because Gtk::Application derives from Gio::ActionMap.

  //File|New sub menu:
  add_action("newstandard",
    sigc::mem_fun(*this, &ImageApp::on_menu_file_new_generic));

  add_action("newfoo",
    sigc::mem_fun(*this, &ImageApp::on_menu_file_new_generic));

  add_action("newgoo",
    sigc::mem_fun(*this, &ImageApp::on_menu_file_new_generic));

  //File menu:
  add_action("quit", sigc::mem_fun(*this, &ImageApp::on_menu_file_quit));

  //Help menu:
  add_action("about", sigc::mem_fun(*this, &ImageApp::on_menu_help_about));

  // Set accelerator keys:
  set_accel_for_action("app.newstandard", "<Primary>n");
  set_accel_for_action("app.quit", "<Primary>q");
  set_accel_for_action("win.copy", "<Primary>c");
  set_accel_for_action("win.paste", "<Primary>v");
  
  //Layout the actions in a menubar and a menu:
  try
  {
    m_refBuilder = Gtk::Builder::create_from_file("UI/menubar.xml");
  }
  catch (const Glib::Error& ex)
  {
    std::cerr << "Building menus failed: " << ex.what();
  }

  //Get the menubar and the app menu, and add them to the application:
  auto gmenu = m_refBuilder->get_object<Gio::Menu>("menubar");
  if (!gmenu)
  {
    g_warning("GMenu not found");
  }
  else
  {
    set_menubar(gmenu);
  }
}

void ImageApp::on_activate()
{
  //std::cout << "debug1: " << G_STRFUNC << std::endl;
  // The application has been started, so let's show a window.
  // A real application might want to reuse this window in on_open(),
  // when asked to open a file, if no changes have been made yet.
  create_window();
}

void ImageApp::create_window()
{
  auto win = new MainWindow();

  //Make sure that the application runs for as long this window is still open:
  add_window(*win);

  //Delete the window when it is hidden.
  win->signal_hide().connect(sigc::bind(
    sigc::mem_fun(*this, &ImageApp::on_window_hide), win));

  win->set_show_menubar();
  win->set_visible(true);
}

void ImageApp::on_window_hide(Gtk::Window* window)
{
  delete window;
}

void ImageApp::on_menu_file_new_generic()
{
  std::cout << "A File|New menu item was selected." << std::endl;
}

void ImageApp::on_menu_file_quit()
{
  std::cout << G_STRFUNC << std::endl;
  quit(); // Not really necessary, when Gtk::Widget::set_visible(false) is called.

  // Gio::Application::quit() will make Gio::Application::run() return,
  // but it's a crude way of ending the program. The window is not removed
  // from the application. Neither the window's nor the application's
  // destructors will be called, because there will be remaining reference
  // counts in both of them. If we want the destructors to be called, we
  // must remove the window from the application. One way of doing this
  // is to hide the window.
  std::vector<Gtk::Window*> windows = get_windows();
  if (windows.size() > 0)
    windows[0]->set_visible(false); // In this simple case, we know there is only one window.
}

void ImageApp::on_menu_help_about()
{
  std::cout << "Help|About App was selected." << std::endl;
}
