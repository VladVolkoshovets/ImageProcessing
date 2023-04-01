#include <iostream>

#include "mainwindow.h"

MainWindow::MainWindow()
: Gtk::ApplicationWindow(),
  m_Box(Gtk::Orientation::VERTICAL)
{
  set_title("Image processing");
  set_default_size(300, 100);

  // ImageApp displays the menubar. Other stuff, such as a toolbar,
  // is put into the box.
  set_child(m_Box);

  // Create actions for menus and toolbars.
  // We can use add_action() because Gtk::ApplicationWindow derives from Gio::ActionMap.
  // This Action Map uses a "win." prefix for the actions.
  // Therefore, for instance, "win.copy", is used in ImageApp::on_startup()
  // to layout the menu.

  //Edit menu:
  add_action("copy", sigc::mem_fun(*this, &MainWindow::on_menu_others));
  add_action("paste", sigc::mem_fun(*this, &MainWindow::on_menu_others));
  add_action("something", sigc::mem_fun(*this, &MainWindow::on_menu_others));

  //Choices menus, to demonstrate Radio items,
  //using our convenience methods for string and int radio values:
  m_refChoice = add_action_radio_string("choice",
    sigc::mem_fun(*this, &MainWindow::on_menu_choices), "a");

  m_refChoiceOther = add_action_radio_integer("choiceother",
    sigc::mem_fun(*this, &MainWindow::on_menu_choices_other), 1);

  m_refToggle = add_action_bool("sometoggle",
    sigc::mem_fun(*this, &MainWindow::on_menu_toggle), false);

  //Help menu:
  add_action("about", sigc::mem_fun(*this, &MainWindow::on_menu_others));

  //Create the toolbar and add it to a container widget:
  try
  {
    m_refBuilder = Gtk::Builder::create_from_file("UI/toolbar.xml");
  }
  catch (const Glib::Error& ex)
  {
    std::cerr << "Building toolbar failed: " <<  ex.what();
  }

  auto toolbar = m_refBuilder->get_widget<Gtk::Box>("toolbar");
  if (!toolbar)
    g_warning("toolbar not found");
  else
    m_Box.append(*toolbar);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_menu_others()
{
  std::cout << "A menu item was selected." << std::endl;
}

void MainWindow::on_menu_choices(const Glib::ustring& parameter)
{
  //The radio action's state does not change automatically:
  m_refChoice->change_state(parameter);

  Glib::ustring message;
  if (parameter == "a")
    message = "Choice a was selected.";
  else
    message = "Choice b was selected.";

  std::cout << message << std::endl;
}

void MainWindow::on_menu_choices_other(int parameter)
{
  //The radio action's state does not change automatically:
  m_refChoiceOther->change_state(parameter);

  Glib::ustring message;
  if (parameter == 1)
    message = "Choice 1 was selected.";
  else
    message = "Choice 2 was selected.";

  std::cout << message << std::endl;
}

void MainWindow::on_menu_toggle()
{
  bool active = false;
  m_refToggle->get_state(active);

  //The toggle action's state does not change automatically:
  active = !active;
  m_refToggle->change_state(active);

  Glib::ustring message;
  if (active)
    message = "Toggle is active.";
  else
    message = "Toggle is not active.";

  std::cout << message << std::endl;
}
