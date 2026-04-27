#include <gtkmm.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <string>
#include <array>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <regex>

std::string exec_command(const std::string& cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) return "";
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

class KeyboardInfoWindow : public Gtk::Window {
public:
    KeyboardInfoWindow(const std::string& default_mac) : m_target_mac(default_mac) {
        set_title("Bluetooth Device🔋Monitor");
        set_default_size(420, 500);
        set_border_width(20);

        m_main_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
        m_main_box.set_spacing(15);
        add(m_main_box);

        // --- Clickable Icon Area ---
        auto event_box = Gtk::make_managed<Gtk::EventBox>();
        // Fixed size request prevents the rest of the UI from moving during zoom
        event_box->set_size_request(-1, 200);

        m_battery_icon.set_from_icon_name("battery-missing-symbolic", Gtk::ICON_SIZE_DIALOG);
        m_battery_icon.set_pixel_size(BASE_SIZE);
        m_battery_icon.set_halign(Gtk::ALIGN_CENTER);

        event_box->add(m_battery_icon);
        event_box->signal_button_press_event().connect([this](GdkEventButton*){
            show_about_dialog();
            return true;
        });
        m_main_box.pack_start(*event_box, Gtk::PACK_SHRINK);

        m_header.set_markup("<span size='x-large' weight='bold'>Bluetooth Device🔋Monitor</span>");
        m_header.set_halign(Gtk::ALIGN_CENTER);
        m_main_box.pack_start(m_header, Gtk::PACK_SHRINK);

        m_sub.set_markup("<span size='medium'>by: mLace\n\n🌐 mlace.com</span>");
        m_sub.set_halign(Gtk::ALIGN_CENTER);
        m_main_box.pack_start(m_sub, Gtk::PACK_SHRINK);

        // --- Data Grid ---
        m_grid.set_column_spacing(20);
        m_grid.set_row_spacing(10);
        m_grid.set_halign(Gtk::ALIGN_CENTER);
        m_main_box.pack_start(m_grid, Gtk::PACK_SHRINK);

        // --- Change MAC Button ---
        m_change_btn.set_label("Change (MAC) Device");
        m_change_btn.signal_clicked().connect(sigc::mem_fun(*this, &KeyboardInfoWindow::on_change_mac_clicked));
        m_main_box.pack_start(m_change_btn, Gtk::PACK_SHRINK);

        m_version_lbl.set_markup("<span color='#666666' size='small'>v0.01a009</span>");
        m_version_lbl.set_halign(Gtk::ALIGN_END);
        m_main_box.pack_end(m_version_lbl, Gtk::PACK_SHRINK);

        refresh_data();
        Glib::signal_timeout().connect([this](){ refresh_data(); return true; }, 30000);
        show_all_children();
    }

private:
    std::string m_target_mac;
    Gtk::Box m_main_box;
    Gtk::Grid m_grid;
    Gtk::Label m_header, m_sub, m_version_lbl;
    Gtk::Image m_battery_icon;
    Gtk::Button m_change_btn;
    const int BASE_SIZE = 160;

    void run_zoom_animation() {
        // Step 1: Shrink to 85%
        m_battery_icon.set_pixel_size(BASE_SIZE * 0.85);

        // Step 2: Enlarge to 120% after 400ms
        Glib::signal_timeout().connect([this]() {
            m_battery_icon.set_pixel_size(BASE_SIZE * 1.2);
            return false;
        }, 400);

        // Step 3: Back to 100% after 800ms
        Glib::signal_timeout().connect([this]() {
            m_battery_icon.set_pixel_size(BASE_SIZE);
            return false;
        }, 800);
    }

    void on_change_mac_clicked() {
        // Disable button (cooldown)
        m_change_btn.set_sensitive(false);
        Glib::signal_timeout().connect([this]() {
            m_change_btn.set_sensitive(true);
            return false;
        }, 2000);

        Gtk::Dialog dialog("New Device", *this, true);
        dialog.set_default_size(300, 150);

        Gtk::Label label("Please enter new MAC address:");
        Gtk::Entry entry;
        entry.set_placeholder_text("xx:xx:xx:xx:xx:xx");
        entry.set_text(m_target_mac);

        dialog.get_content_area()->pack_start(label, Gtk::PACK_SHRINK, 10);
        dialog.get_content_area()->pack_start(entry, Gtk::PACK_SHRINK, 10);

        dialog.add_button("Cancel", Gtk::RESPONSE_CANCEL);
        Gtk::Button* apply_btn = dialog.add_button("Apply", Gtk::RESPONSE_OK);

        auto validate = [&]() {
            std::regex mac_regex("^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$");
            apply_btn->set_sensitive(std::regex_match(entry.get_text().raw(), mac_regex));
        };

        entry.signal_changed().connect(validate);
        validate();

        dialog.show_all_children();
        if (dialog.run() == Gtk::RESPONSE_OK) {
            m_target_mac = entry.get_text();
            refresh_data();
        }
    }

    void show_about_dialog() {
        Gtk::AboutDialog dialog;
        dialog.set_transient_for(*this);
        dialog.set_program_name("Bluetooth Device🔋Monitor");
        dialog.set_version("v0.01a009");

        // 1. Set the icon name as usual
        dialog.set_logo_icon_name("battery-symbolic");

        // 2. Find the Logo widget and apply margins
        // We search the content area for the Gtk::Image that holds our logo
        auto content_area = dialog.get_content_area();
        std::vector<Gtk::Widget*> children = content_area->get_children();

        for (auto child : children) {
            // The logo is usually inside a container or is a direct Image child
            if (auto img = dynamic_cast<Gtk::Image*>(child)) {
                img->set_margin_top(30);    // Adjust these values to your liking
                img->set_margin_bottom(20);
            }
        }

        dialog.set_website("http://mlace.com");
        dialog.set_copyright("by: mLace\n\n🌐 mlace.com\n\nSmall App initially created to check\nthe FAVI Bluetooth Wireless Keyboard\nbattery level.\n\nUpdate Bonus: Any wireless device's MAC address can\nbe entered to view it's battery level!\n\n");
        dialog.set_license_type(Gtk::LICENSE_GPL_3_0);

        dialog.run();
    }

    void update_icon(int percentage, const std::string& state) {
        std::string icon_name;
        bool is_charging = (state == "charging" || state == "fully-charged");

        // --- Handle "Not Found" or "Searching" ---
        if (state == "searching" || state.empty()) {
            icon_name = "battery-missing-symbolic";
            m_battery_icon.set_from_icon_name(icon_name, Gtk::ICON_SIZE_DIALOG);
            m_battery_icon.unset_color(); // Reset color to default grey/theme color
            return;
        }

        // --- Handle Percentage Icons ---
        if (state == "empty") {
            icon_name = "battery-level-0-symbolic";
        } else if (!is_charging) {
            if (percentage <= 10) icon_name = "battery-10-symbolic";
            else if (percentage <= 20) icon_name = "battery-caution-symbolic";
            else if (percentage <= 29) icon_name = "battery-level-20-symbolic";
            else if (percentage <= 39) icon_name = "battery-low-symbolic";
            else if (percentage <= 55) icon_name = "battery-level-50-symbolic";
            else if (percentage <= 63) icon_name = "battery-level-60-symbolic";
            else if (percentage <= 72) icon_name = "battery-level-70-symbolic";
            else if (percentage <= 86) icon_name = "battery-level-80-symbolic";
            else if (percentage <= 97) icon_name = "battery-level-90-symbolic";
            else icon_name = "battery-level-100-symbolic";
        } else {
            if (percentage == 0) icon_name = "battery-empty-charging-symbolic";
            else if (percentage <= 10) icon_name = "battery-10-charging-symbolic";
            else if (percentage <= 20) icon_name = "battery-20-charging-symbolic";
            else if (percentage <= 29) icon_name = "battery-level-30-charging-symbolic";
            else if (percentage <= 39) icon_name = "battery-level-40-charging-symbolic";
            else if (percentage <= 55) icon_name = "battery-level-50-charging-symbolic";
            else if (percentage <= 63) icon_name = "battery-level-60-charging-symbolic";
            else if (percentage <= 72) icon_name = "battery-level-70-charging-symbolic";
            else if (percentage <= 86) icon_name = "battery-level-80-charging-symbolic";
            else if (percentage <= 97) icon_name = "battery-level-90-charging-symbolic";
            else icon_name = "battery-level-100-charging-symbolic";
        }

        m_battery_icon.set_from_icon_name(icon_name, Gtk::ICON_SIZE_DIALOG);

        // Apply colors based on health
        if (percentage < 20) m_battery_icon.override_color(Gdk::RGBA("red"));
        else if (percentage < 50) m_battery_icon.override_color(Gdk::RGBA("yellow"));
        else m_battery_icon.override_color(Gdk::RGBA("green"));
    }

    void refresh_data() {
        for (auto child : m_grid.get_children()) m_grid.remove(*child);

        std::string find_path_cmd = "upower -e | xargs -I {} sh -c 'upower -i {} | grep -qi " + m_target_mac + " && echo {}'";
        std::string path = exec_command(find_path_cmd);
        path.erase(path.find_last_not_of("\n\r \t") + 1);

        if (path.empty()) {
            m_header.set_text("Searching: " + m_target_mac);
            update_icon(0, "searching"); // Explicitly trigger the missing icon
            return;
        }

        // Only trigger the zoom animation if the device is actually found
        run_zoom_animation();

        std::string raw_data = exec_command("upower -i " + path);
        std::stringstream ss(raw_data);
        std::string line, state_val = "";
        int percent_val = 0, row = 0;

        while (std::getline(ss, line)) {
            if (line.find(":") != std::string::npos) {
                std::string k = line.substr(0, line.find(":")), v = line.substr(line.find(":") + 1);
                k.erase(0, k.find_first_not_of(" ")); v.erase(0, v.find_first_not_of(" "));
                if (k == "model" || k == "percentage" || k == "state") {
                    m_grid.attach(*Gtk::make_managed<Gtk::Label>("<weight=bold>"+k+":</weight>", true), 0, row, 1, 1);
                    m_grid.attach(*Gtk::make_managed<Gtk::Label>(v), 1, row++, 1, 1);
                    if (k == "percentage") percent_val = std::atoi(v.c_str());
                    if (k == "state") state_val = v;
                }
            }
        }
        update_icon(percent_val, state_val);
        show_all_children();
    }
};

int main(int argc, char* argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.mlace.btmonitor");
    KeyboardInfoWindow window("54:46:6B:01:96:85");
    return app->run(window);
}

