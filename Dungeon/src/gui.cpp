#include "gui.hpp"
#include "character.hpp"
#include <unistd.h>
#include <ncurses.h>
#include <sstream>
#include <cstring>
#include <iostream>
#include "bomb.hpp"

#define NOISE_CHARACTER  '#'

Game *GUI::g;
Point GUI::cam;
int GUI::NUM_COLOURS;
std::string GUI::status_line;
std::vector<std::string> GUI::messages;
const char *GUI::SplashStr = "\
\n\
\n\
\n\
\n\
\n\
88888888888 888                     8888888b.                                                        \
    888     888                     888   Y88b                                                       \
    888     888                     888    888                                                       \
    888     8888b.    .d88b.        888    888 888  888 88888b.   .d88b.   .d88b.   .d88b.  88888b.  \
    888     888  88b d8P  Y8b       888    888 888  888 888  88b d88P 88b d8P  Y8b d88  88b 888  88b \
    888     888  888 88888888       888    888 888  888 888  888 888  888 88888888 888  888 888  888 \
    888     888  888 Y8b.           888  .d88P Y88b 888 888  888 Y88b 888 Y8b.     Y88..88P 888  888 \
    888     888  888   Y8888        8888888P     Y88888 888  888   Y88888   Y8888    Y88P   888  888 \
                                                                      888                            \
                                                                 Y8b d88P                            \
                                                                   Y88P                              \
\n\
\n\
\n\
\n\
\n\
";

void
GUI::Init(void) {
    initscr();
	cbreak();
	keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    start_color();

    if (COLS < 80 || LINES < 24) {
        endwin();
        std::cout << "ERROR: This game requires a terminal size of at least 80x24\n";
        exit(1);
    }

    for (int i = 0; i < NUM_COLOURS; ++i) 
        init_pair(i, i, COLOR_BLACK);
}

void
GUI::End(void) {
    endwin();
}

void
GUI::AttachTo(Game *g) {
    GUI::g = g;
}

void
GUI::RedrawStatus(void) {
    mvprintw(LINES -1, 0, GUI::status_line.c_str());
    for (int i = GUI::status_line.length(); i <= COLS; ++i)
        addch(' ');
}

void
GUI::SetStatus(std::string str) {
    GUI::status_line = str;
    GUI::RedrawStatus();
}

void
GUI::AddMessage(std::string str) {
    messages.push_back(str);
}

void
GUI::CharacterStatusLine(Character *c) {
    std::stringstream ss;

    ss <<  "HP:" << c->curHp << "/" << c->baseHp;
    ss << "    GP:" << c->getGold();
    ss << "    ATT:" << c->curAttack;
    ss << "    DEF:" << c->curDef;

    GUI::SetStatus(ss.str());
}

void
GUI::ProcessMessages(Game *g) {
    std::string s;

    if (messages.size() == 0) {
        GUI::CharacterStatusLine(g->character);
        return;
    }

    for (std::vector<std::string>::iterator it = messages.begin(); 
            it != messages.end();) {
        s = *it;
        SetStatus(s);
        if (++it != messages.end()) {
            mvprintw(LINES - 1, s.length(), " <More>");
            getch();
        }
    }
    messages.clear();
}

Character*
GUI::CharacterCreation(void) {
    Character *c;
    std::string name;

    GUI::ShowSplash();
    name = GUI::GetString("Enter your character name:");

    c = new Character(name);
    if (c->name == "") {
        std::string promt_str("You did not name your character, so now he is called ");
        auto rand_name = c->GiveRandomName();
        promt_str = promt_str + rand_name;

        GUI::Alert(promt_str.c_str());
    }

    return c;
}

std::string
GUI::GetString(std::string prompt) {
    static const int IN_SZ = 30;
    WINDOW *w;
    char in[IN_SZ];
    std::stringstream ss;
    std::string s;

    echo();
    curs_set(1);
    refresh();
    w = GUI::NewCentredWindow(IN_SZ + prompt.length() + 6, 3);
    if (!w) {
        std::cerr << "Error making window for string selection!";
        return "";
    }
    box(w, 0, 0);
    mvwprintw(w, 1, 2, prompt.c_str());
    waddch(w, ' ');

    wrefresh(w);
    refresh();
    wgetnstr(w, in, 30);

    noecho();
    curs_set(0);
    delwin(w);

    ss << in;
    s = ss.str();
    return s;
}


void
GUI::ShowSplash(void) {
    GUI::ShowSplash(COL_BLUE);
}

void
GUI::ShowSplash(Colour col) {
    WINDOW *w;

    refresh();

    w = GUI::NewCentredWindow(101, 20);
    wattron(w, COLOR_PAIR(col));
    mvwprintw(w, 0, 0, SplashStr);

    wattroff(w, COLOR_PAIR(col));
    wrefresh(w);
    refresh();
    delwin(w);
}

void
GUI::StartScreen(void) {
    static const std::string request = "Press any key to begin...";
    GUI::ShowSplash(COL_RED);
    mvprintw(LINES/2 + 10, ((COLS - request.length()) / 2), request.c_str());
    getch();
}

void
GUI::ScreenNoise(void) {
    for (int e = 0; e < LINES; ++e) {
        for (int i = 0; i < COLS; ++i) {
            mvaddch(e, i, '#'); 
        }
    }
}

WINDOW*
GUI::NewCentredWindow(int w, int h) {
    return newwin(h, w, (LINES - h) / 2, (COLS - w) / 2);
}

void
GUI::Alert(std::string str) {
    WINDOW *w;
    int c;
    Rect pos;
    static std::string const press_msg = "Press spacebar to continue.";

    pos.w = MAX(str.length(), press_msg.length()) + 4;
    pos.w = MIN(pos.w, 80);
    pos.h = 5;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for an alert!";
        return;
    }
    box(w, 0, 0);

    wrefresh(w);
    refresh();

    pos.x = (pos.w - str.length()) / 2;
    mvwprintw(w, 2, pos.x, str.c_str());
    pos.x = (pos.w - press_msg.length()) / 2;
    mvwprintw(w, 4, pos.x, press_msg.c_str());
    wrefresh(w);
    refresh();

    while((c = wgetch(w)) != ' ');

    delwin(w);
}

void
GUI::Alert2(char const * const * const msg) {
    WINDOW *w;
    int c;
    Rect pos;
    static std::string const press_msg = "Press spacebar to continue.";
    int maxlen;
    int lines;

    maxlen = press_msg.length();
    for (lines = 0; msg[lines]; ++lines) {
        if ((int)strlen(msg[lines]) > maxlen)
            maxlen = strlen(msg[lines]);
    }

    pos.w = MIN(maxlen + 4, 80);
    pos.h = 5 + lines - 1;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for an alert!";
        return;
    }
    box(w, 0, 0);

    wrefresh(w);
    refresh();

    for (int i = 0; i < lines; ++i) {
        pos.x = (pos.w - strlen(msg[i])) / 2;
        mvwprintw(w, 2+i, pos.x, msg[i]);
    }
    pos.x = (pos.w - press_msg.length()) / 2;
    mvwprintw(w, pos.h - 1, pos.x, press_msg.c_str());
    wrefresh(w);
    refresh();

    while((c = wgetch(w)) != ' ');

    delwin(w);
}

bool
GUI::BinaryChoice(std::string str, char a, char b) {
    WINDOW *w;
    Rect pos;
    char c;

    str.append(1, ' ');
    str.append(1, a);
    str.append(1, '/');
    str.append(1, b);

    pos.w = str.length() + 4;
    pos.h = 3;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for a binary choice! (Defaulted to true)";
        return true;
    }
    box(w, 0, 0);
    pos.x = (pos.w - str.length()) / 2;
    mvwprintw(w, 1, pos.x, str.c_str());

    wrefresh(w);

    while(1) {
        c = getch();

        if (c == a || c == b)
            break;
    }

    delwin(w);

    if (c == a)
        return true;
    return false;
}

void
GUI::FancyClear(void) {
    wmove(stdscr, 0, 0);
    for (int i = 0; i < LINES - 1; ++i) {
        for (int e = 0; e < COLS; ++e) {
            waddch(stdscr, ' ');
        }
    }
    refresh();
}

void
GUI::CharacterRename(Character *c) {
    std::string newname;

    newname = GUI::GetString("Enter the new character name: ");
    if (newname == "") {
        GUI::Alert("You can't have an empty name. Using the old name.");
        return;
    }
    c->name = newname;
}

void
GUI::ShowCharacterScreen(Character *c) {
    WINDOW *w;
    Rect pos;
    std::stringstream ss;
    std::string s;
    Point p;

    pos.w = 80;
    pos.h = 24;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    if (!w) {
        std::cerr << "Error making window for character screen!";
        return;
    }
    box(w, 0, 0);

    wmove(w, 3, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, 0, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, pos.h - 1, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }

    mvwprintw(w, 1, (pos.w - c->name.length()) / 2, c->name.c_str());

    p.x = 3;
    p.y = 5;
    ss.str("");
    ss << "HP  :: " << c->curHp << "/" << c->baseHp;
    s = ss.str();
    mvwprintw(w, p.y, p.x, s.c_str());

    ss.str("");
    ss << "Attack :: " << c->curAttack;
    if (c->curAttack != c->baseAttack)
        ss << "\t(Base :: " << c->baseAttack << ")";
    s = ss.str();
    mvwprintw(w, ++p.y, p.x, s.c_str());

    ss.str("");
    if (!w) {
        std::cerr << "Error making window for inventory screen!";
        return;
    }

    mvwprintw(w, p.y += 2, p.x -1, "=|==|==|==|==|==|==|==|==|==|==|==|=");

    mvwprintw(w, p.y + 2, p.x - 1, "=|==|==|==|==|==|==|==|==|==|==|==|==|=");

    p.y = 10;

    int i = 0;
    for (std::vector<Equippable*>::iterator it = c->equip.begin(); 
        it != c->equip.end(); it++) {
        ss.str("");
        ss << i++ << " :: ";
        if(*it != nullptr) ss << (*it)->GetName();
        else ss << "NONE";
        s = ss.str();
        mvwprintw(w, p.y, p.x, s.c_str());
    }

    wrefresh(w);
    delwin(w);
}

void
GUI::ShowInventoryScreen(Character *c) {
    WINDOW *w;
    Rect pos;
    std::stringstream ss;
    std::string s;
    Item* item;
    int i;

    pos.w = 80;
    pos.h = 24;
    pos.x = (COLS - pos.w) / 2;
    pos.y = (LINES - pos.h) / 2;

    w = newwin(pos.h, pos.w, pos.y, pos.x);
    box(w, 0, 0);

    wmove(w, 0, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, 2, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }
    wmove(w, pos.h - 1, 1);
    for (int i = 0; i < pos.w - 2; ++i) {
        if ((i - 1) % 3 == 0)
            waddch(w, '|');
        else
            waddch(w, '=');
    }

    mvwprintw(w, 1, 2, "INVENTORY");

    ss << "Wealth :: " << c->getGold() << "gp";
    s = ss.str();
    mvwprintw(w, 1, pos.w - 2 - s.length(), s.c_str());

    i = 0;
    for (std::list<Item*>::iterator it = c->Inventory.begin();
            it != c->Inventory.end(); ++it, ++i) {
        item = &**it;
        mvwprintw(w, i + 3, 8, item->GetName().c_str());
    }

    wrefresh(w);
    delwin(w);
}

void
GUI::QuitDialogue(Game *g) {
    GUI::FancyClear();
    GUI::ShowSplash();
    g->running = !GUI::BinaryChoice("Are you sure you want to quit?", 'y', 'n');
    if (g->running)
        GUI::DoRedraw();
}

void
GUI::DrawAsOverlay(Point p, char c, int col) {
    if (!GUI::isOnScreen(p))
        return;
    attron(COLOR_PAIR(col));
    mvaddch(p.y - GUI::cam.y, p.x - GUI::cam.x, c);
    attroff(COLOR_PAIR(col));
}

bool
GUI::isOnScreen(Point p) {
    if (p.x - GUI::cam.x < 0)
        return false;
    if (p.y - GUI::cam.y < 0)
        return false;
    if (p.x - GUI::cam.x >= COLS)
        return false;
    if (p.y - GUI::cam.y >= LINES - 1)
        return false;

    return true;
}

void
GUI::MoveCamera(Direction::Type d) {
    int step_x;
    int step_y;

    step_x = MAX(COLS / 3, 1);
    step_y = MAX(LINES / 3, 1);

    switch (d) {
        case Direction::NORTH:
            GUI::cam.y -= step_y;
            break;
        case Direction::SOUTH:
            GUI::cam.y += step_y;
            break;
        case Direction::EAST:
            GUI::cam.x += step_x;
            break;
        case Direction::WEST:
            GUI::cam.x -= step_x;
            break;
        default:
            break;
    }

    GUI::DoRedraw();
}

void
GUI::DoRedraw(void) {
    if (g->game_mode == GameMode::MAP_WALK) {
        GUI::DrawLevel(g->cur_level);
        GUI::RedrawStatus();
    }
    else if (g->game_mode == GameMode::INVENTORY_SCREEN)
        GUI::ShowInventoryScreen(g->character);
    else if (g->game_mode == GameMode::SPELL_SELECT)
        g->SpellSelectMenu->Show();
    else if (g->game_mode == GameMode::GEAR_SELECT)
        g->EquipSelectMenu->Show();
    else if (g->game_mode == GameMode::SPELL_SELECT)
        g->SpellSelectMenu->Show();
}

void
GUI::DrawLevel(Level *l) {
    int e2, i2;
    Item *item;
    Tile *t;
    char c;
    Colour colour;

    move(0,0);

    for (int e = 0; e < LINES - 1; ++e) {
        for (int i = 0; i < COLS; ++i) {
            i2 = i + GUI::cam.x;
            e2 = e + GUI::cam.y;
            t = &l->tiles[i2][e2];
            if (i2 >= MAP_W || i2 < 0 || e2 >= MAP_H || e2 < 0)
                c = ' ';
            else if (!t->isVisible)
                c = ' ';
            else
                c = t->getTileType();

            if (c == CLOSED_DOOR_CHAR || c == OPEN_DOOR_CHAR) {
                attron(COLOR_PAIR(COL_RED));
                addch(c);
                attroff(COLOR_PAIR(COL_RED));
            }
            else if (c == FLOOR_CHAR) {
                if (GUI::NUM_COLOURS < 16)
                    colour = COL_WHITE;
                else
                    colour = COL_GREY;
                attron(COLOR_PAIR(colour));
                addch(c);
                attroff(COLOR_PAIR(colour));
            }
            else
                addch(c);
        }
    }

    for (std::list<Item*>::iterator it = l->items.begin();
            it != l->items.end(); it++) {
        item = &**it;
        l->ConditionallyShowObject(item->pos, item->symbol, item->colour);
    }

    //Enemies
    for (std::list<Enemy>::iterator it = l->enemies.begin();
            it != l->enemies.end(); it++) {
        l->ConditionallyShowObject(it->pos, it->getSymbol(), it->getColour());
    }

    //Projectiles
    for (std::vector<Projectile*>::iterator it = l->projs.begin();
            it != l->projs.end(); it++) {
        l->ConditionallyShowObject(*(*it)->p, (*it)->symbol, (*it)->c);
    }

    //Bombs
    for (std::vector<Bomb*>::iterator it = l->bombs.begin();
            it != l->bombs.end(); it++) {
        l->ConditionallyShowObject((*it)->p, (*it)->symbol, (*it)->colour);
    }

    //Character
    l->ConditionallyShowObject(GUI::g->character->pos, GUI::g->character->symbol,
        GUI::g->character->colour);

    refresh();
}

void
GUI::DrawObjectRelative(Point p, char c) {
    mvaddch(p.y - GUI::cam.y, p.x - GUI::cam.x, c);
}

void
GUI::CentreCam(Point p) {
    GUI::cam.x = p.x - COLS / 2;
    GUI::cam.y = p.y - LINES / 2;
}

void
GUI::HardRedraw(void) {
    erase();
    clear();
    endwin();
    refresh();
    GUI::DoRedraw();
}

void
GUI::ShowTombstone(Game *g) {
    std::stringstream ss;
    std::string s;
    Character *c;
    WINDOW *w;

    c = g->character;
    w = GUI::NewCentredWindow(80, 24);

    ss.str("");
    ss << c->name;
    ss.str("");
    ss << "Monsters killed: " << c->monsters_killed;
    s = ss.str();
    mvwprintw(w, 11, (80 - s.length()) / 2, s.c_str());

    ss.str("");
    ss << "Gold collected: " << c->total_gold_collected;
    s = ss.str();
    mvwprintw(w, 13, (80 - s.length()) / 2, s.c_str());

    GUI::FancyClear();
    GUI::SetStatus("");
    wrefresh(w);
    refresh();
    delwin(w);
}

Direction::Type
GUI::GetDirection(void) {
    int c;

    GUI::DrawLevel(g->cur_level);
    GUI::SetStatus("Cast in which direction?");
    while (1) {
        c = getch();
        switch (c) {
            case '7':
            case 'q':
                return Direction::NW;
            case '8':
            case 'w':
                return Direction::NORTH;
            case '9':
            case 'e':
                return Direction::NE;
            case '4':
            case 'a':
                return Direction::WEST;
            case '6':
            case 'd':
                return Direction::EAST;
            case '1':
            case 'z':
                return Direction::SW;
            case '2':
            case 'x':
                return Direction::SOUTH;
            case '3':
            case 'c':
                return Direction::SE;
            default:
                break;
        }
    }
    return Direction::LAST_DIRECTION;
}
