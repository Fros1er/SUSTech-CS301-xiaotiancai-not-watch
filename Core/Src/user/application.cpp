#include "application.h"

#include <string>

static lv_style_t titlebar_style, bg_style;
std::unordered_map<uint8_t, std::vector<std::string>> Application::room_msg_map;
std::unordered_map<uint8_t, uint32_t> Application::room_msg_idx_map;
std::unordered_map<uint8_t, uint8_t> Application::room_online_map;
char Application::device_name[4]={0, 0, 0, 0};
uint8_t Application::work_mode = NA_MODE;
uint32_t Application::packet_send_cnt = 0;
uint32_t Application::packet_recv_cnt = 0;
uint8_t Application::tmp_buf[33];
const char* table = "0123456789abcedf";
uint8_t need_init = 1;
extern uint8_t TX_ADDRESS[TX_ADR_WIDTH]; //  = {0x34, 0xde, 0x10, 0x10, 0x01}; /* 发送地址 */
extern uint8_t RX_ADDRESS[RX_ADR_WIDTH];

void uart_transmit_message(const char* msg){
    uart_transmit_dma((uint8_t*)msg, strlen(msg));
}

Application::Application(const std::string &name)
    : _name{name} {
    _bg = lv_obj_create(lv_scr_act());
    lv_obj_add_style(_bg, &bg_style, 0);
    lv_obj_align(_bg, LV_ALIGN_OUT_TOP_MID, 0, MENU_BAR_HEIGHT);
    lv_obj_set_size(_bg, MY_DISP_HOR_RES, MY_DISP_VER_RES - MENU_BAR_HEIGHT);
    lv_obj_add_flag(_bg, LV_OBJ_FLAG_HIDDEN);
    // init static vars
    if (need_init){
        need_init = 0;
        uint32_t chip_id[3];
        chip_id[0] = *(uint32_t*)0x1FFFF7E8;
        chip_id[1] = *(uint32_t*)0x1FFFF7EC;
        chip_id[2] = *(uint32_t*)0x1FFFF7F0;
        uint32_t device_hash = chip_id[0] ^ chip_id[1] ^ chip_id[2];
        device_hash ^= device_hash>>8;
        device_hash ^= device_hash>>16;
        device_hash ^= device_hash>>24;
        device_hash = (device_hash+1)&0xfe;

        device_hash = 0x01;
        device_name[3]=device_hash;
        device_name[0]=table[device_hash>>4];
        device_name[1]=table[device_hash&0xf];
        RX_ADDRESS[1]=device_hash;

        room_msg_map = std::unordered_map<uint8_t, std::vector<std::string>>();
        room_msg_idx_map = std::unordered_map<uint8_t, uint32_t>();
        room_online_map = std::unordered_map<uint8_t, uint8_t>();
        room_msg_map[0] = std::vector<std::string>();
        room_msg_idx_map[0] = 0;
        room_msg_map[0].push_back("Public Chat Begins...");  // 😁😄😝😜🤩🥺🤔😚
    }
}

void Application::make_room(uint8_t room){
    if (!room_msg_map.count(room)){
        room_msg_map[room] = std::vector<std::string>();
        char room_name[3];
        room_name[0]=table[room>>4];
        room_name[1]=table[room&0xf];
        room_name[2]=0;
        room_msg_map[room].push_back(std::string("Chat with ") + std::string(room_name) + std::string(" Begins..."));
        room_msg_idx_map[room] = 0;
    }
}

uint8_t Application::recv_msg(){
    if (work_mode != RX_MODE) {
        nrf24l01_rx_mode();
        work_mode = RX_MODE;
    }
    if(nrf24l01_rx_packet(tmp_buf)){
        return 1;
    } else {
        packet_recv_cnt++;
        if (tmp_buf[1] != 0){ 
            if (RX_ADDRESS[1]!=0xff){  // ping
                if (room_online_map[tmp_buf[0]] == 0){
                    make_room(tmp_buf[0]);
                    // TODO: ALERT ONLINE
                }
                room_online_map[tmp_buf[0]] = 100;
            } else {                 // to server
                uart_transmit_message("[Server A]\n");
                uart_transmit_dma(tmp_buf, 32);
                uart_transmit_message("[Server B]\n");
            }
        } else {
            if (tmp_buf[0] != 0xff){
                room_msg_map[tmp_buf[0]].push_back(std::string((char*)tmp_buf+2));
                room_msg_idx_map[tmp_buf[0]] = room_msg_map[tmp_buf[0]].size()-1;
            } else {
                // sever message
            }
        } 
        return 0;
    }
}

uint8_t Application::send_msg(const char* msg_ptr, uint8_t addr, uint8_t server) {
    int msg_len = strlen(msg_ptr);
    TX_ADDRESS[1]=addr;

    for (int i = 0; i < msg_len; i+=30) {
        tmp_buf[0] = device_name[3];
        tmp_buf[1] = server;
        strncpy((char*)tmp_buf+2, msg_ptr, 30);
        tmp_buf[32] = 0;
        msg_ptr += 30;
        
        if (work_mode != TX_MODE) {
            nrf24l01_tx_mode();
            work_mode = TX_MODE;
        }
        
        for (int retry_cnt = 0; retry_cnt < 13; retry_cnt++){
            if(nrf24l01_tx_packet(tmp_buf)==0) {
                packet_send_cnt++;
                return 0;
            }else {
                DWT_Delay_us(893);
            }
        }
    }
    return 1;
}

Menu::Menu()
    : Application("Menu") {
    info_label = lv_label_create(_bg);
    lv_obj_align(info_label, LV_ALIGN_OUT_TOP_MID, 0, MENU_BAR_HEIGHT);
    // lv_label_set_long_mode(info_label, LV_LABEL_LONG_SROLL_CIRC); 
    lv_obj_set_size(info_label, 220, 270);
    lv_label_set_recolor(info_label, true);
    lv_label_set_text_fmt(info_label, "#0000ff %s# #ff00ff |ID:%s|# #ff0000 %s#", "|Time:here|", device_name, "|XTC");
}

static void event_handler(lv_event_t *e) {
    ApplicationFSM::instance().switch_to(*static_cast<const std::string *>(e->user_data));
}

void Menu::register_application(const std::string &name) {
    lv_obj_t *btn = lv_btn_create(_bg);
    lv_obj_set_pos(btn, 0, 20 + 50 * (app_num));
    lv_obj_add_event_cb(btn, event_handler, LV_EVENT_CLICKED, (void *)&name);

    lv_obj_t *label = lv_label_create(btn);
    lv_label_set_text(label, name.c_str());
    lv_obj_center(label);
    app_num++;
}

static void back_to_menu(lv_event_t *e) {
    ApplicationFSM::instance().switch_to("Menu");
}

ApplicationFSM::ApplicationFSM() {
}

void ApplicationFSM::init() {
    menu = new Menu();
    applications.emplace("Menu", menu);
    _title_bar = lv_obj_create(lv_scr_act());
    lv_obj_align(_title_bar, LV_ALIGN_OUT_TOP_MID, 0, 0);
    lv_obj_set_size(_title_bar, MY_DISP_HOR_RES, MENU_BAR_HEIGHT);

    lv_style_init(&titlebar_style);
    lv_style_set_bg_color(&titlebar_style, lv_color_hex(0xB3E5FC));
    lv_style_set_border_width(&titlebar_style, 0);
    lv_style_set_radius(&titlebar_style, 0);
    lv_obj_add_style(_title_bar, &titlebar_style, 0);
    lv_obj_clear_flag(_title_bar, LV_OBJ_FLAG_SCROLLABLE);

    _title = lv_label_create(_title_bar);
    lv_obj_set_align(_title, LV_ALIGN_CENTER);

    _back_btn = lv_btn_create(_title_bar);
    lv_obj_add_event_cb(_back_btn, back_to_menu, LV_EVENT_CLICKED, NULL);
    lv_obj_align(_back_btn, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_set_height(_back_btn, MENU_BAR_HEIGHT - 10);
    lv_obj_set_style_shadow_width(_back_btn, 0, 0);
    lv_obj_t *label = lv_label_create(_back_btn);
    lv_label_set_text(label, "Menu");
    lv_obj_center(label);

    lv_style_init(&bg_style);
    lv_style_set_border_width(&bg_style, 0);
    lv_style_set_radius(&bg_style, 0);
}

void ApplicationFSM::register_application(Application *app) {
    applications.emplace(app->_name, app);
    menu->register_application(app->_name);
}

void ApplicationFSM::switch_to(const std::string &name) {
    if (cur_app != nullptr) {
        lv_obj_add_flag(cur_app->_bg, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_background(cur_app->_bg);
        cur_app->exit();
    }
    cur_app = applications[name];
    lv_obj_clear_flag(cur_app->_bg, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(cur_app->_bg);
    cur_app->enter();
    lv_label_set_text_static(_title, cur_app->_name.c_str());
    if (cur_app == menu) {
        lv_obj_add_flag(_back_btn, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_clear_flag(_back_btn, LV_OBJ_FLAG_HIDDEN);
    }
}

void ApplicationFSM::tick() {
    if (cur_app != nullptr) {
        cur_app->tick();
        for(auto it=cur_app->room_online_map.begin(), ite=cur_app->room_online_map.end(); it!=ite; it++){
            if (it->second > 0){
                it->second--;
                if (it->second == 0){
                    // TODO: ALERT OFFLINE
                }
            }
        }
        for(int i=0;i<73;i++){
            if(cur_app->recv_msg()==0) {
                break;
            }else {
                DWT_Delay_us(50);
            }
        }
        

        if (RX_ADDRESS[1] == 0xff){
            extern uint8_t uart_buf[UART_BUF_SIZE];
            if (!strncmp((char*)uart_buf, "[PACKET]", 8)) {
                memset(uart_buf, 0, 8);
                uart_transmit_message("YEs i got it!");
            }
        } else {
            
            for (int i=1;i!=cur_app->device_name[3]&&i<4;i++){
                for (int retry=1;retry;retry--){
                    if (cur_app->send_msg("Hello, I'm online.", i, 1)==0){
                        break;
                    }
                }
            }
        }
    }
}
