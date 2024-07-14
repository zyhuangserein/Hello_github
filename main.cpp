#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;

class Skill{
public:
    string name;
    string type;
    string property;
    int power;
    int accuracy;
    int priority;

    Skill(string name, string type, string property, int power, int accuracy, int priority)
        : name(name), type(type), property(property), power(power), accuracy(accuracy), priority(priority) {}
};

class Slime{
public:
    string name;
    string property;
    int max_HP;
    int current_HP;
    int attack_power;
    int defense_power;
    int speed;
    vector<Skill> skills;

    Slime(string name, string property, int max_HP, int current_HP, int attack_power, int defense_power, int speed, Skill skill_1, Skill skill_2)
        : name(name), property(property), max_HP(max_HP), current_HP(max_HP), attack_power(attack_power), defense_power(defense_power), speed(speed) {
        skills.push_back(skill_1);
        skills.push_back(skill_2);
    }

    //�ж�ʷ��ķ�Ƿ����
    bool is_alive() {
        return current_HP > 0;
    }

    //ʷ��ķ�ڶ�����ʹ�ü��ɲ����ع���ֵ
    int use_skill(Skill skill, Slime& target) {
        int harm;
        double property_multiply;
        if (skill.property == "Ordinary" || target.property == "Ordinary") {
            property_multiply = 1.0;
        } else if ((skill.property == "Fire" && target.property == "Grass") ||
                   (skill.property == "Water" && target.property == "Fire") ||
                   (skill.property == "Grass" && target.property == "Water")) {
            property_multiply = 2.0;
        } else {
            property_multiply = 0.5;
        }
        harm = round((skill.power) * (attack_power) / (target.defense_power) * (property_multiply));
        target.current_HP -= harm;

        return harm;
    }
};

class SimpleEnemy{
public:

    //�򵥵з�ѡȡʷ��ķ����
    Slime* choose_next_slime(Slime* player_slime, vector<Slime>& enemy) {
        for (auto& slime : enemy) {
            if (!slime.is_alive()) continue;

            if ((player_slime->property == "Grass" && slime.property == "Fire") ||
                (player_slime->property == "Fire" && slime.property == "Water") ||
                (player_slime->property == "Water" && slime.property == "Grass")) {
                return &slime; // ���ȿ���
            }
        }
        for (auto& slime : enemy) {
            if (!slime.is_alive()) continue;

            if (player_slime->property == slime.property) {
                return &slime; // ���ѡ����ͬ����
            }
        }
        for (auto& slime : enemy) {
            if (slime.is_alive()) {
                return &slime;//�����ŵļ���
            }
        }
        return nullptr;
    }

    //�򵥵з�ѡ���ж�����(����з����ϵ�ʷ��ķ������ҳ��ϵ�ʷ��ķ����ô�з�ѡ���ø�ʷ��ķʹ�ü���2������ѡ��ʹ�ü���1)
    string choose_action(Slime& player_slime, Slime& enemy_slime) {
        if ((player_slime.property == "Grass" && enemy_slime.property == "Fire") ||
            (player_slime.property == "Fire" && enemy_slime.property == "Water") ||
            (player_slime.property == "Water" && enemy_slime.property == "Grass"))
            return "skill_2";

        return "skill_1";
    }


};

class Battle{
public:
    vector<Slime> player;
    vector<Slime> enemy;
    int round;
    SimpleEnemy enemy_strategy;

    Battle(vector<Slime> player, vector<Slime> enemy)
        : player(player), enemy(enemy), round(1) {}

    void start_battle() {
        cout << "Welcome to Battle of Slimes!" << endl;
        cout << "You have Green, Red and Blue. So does Enemy." << endl;
        cout << "Select your starting slime (1 for Green, 2 for Red, 3 for Blue): ";
        int starting_slime;
        cin >> starting_slime;
        while (starting_slime != 1 && starting_slime != 2 && starting_slime != 3) {
            cout << "Select your starting slime (1 for Green, 2 for Red, 3 for Blue): ";
            cin >> starting_slime;
        }
        Slime* player_slime = &player[starting_slime - 1];
        Slime* enemy_slime = enemy_strategy.choose_next_slime(player_slime, enemy);
        cout << "You start with " << player_slime->name << endl;
        cout << "Enemy starts with " << enemy_slime->name << endl;
        cout << "Your " << player_slime->name << ": HP " << player_slime->current_HP << " || "
             << "Enemy's " << enemy_slime->name << ": HP " << enemy_slime->current_HP << endl;
        cout << "Battle starts!" << endl;
        cout << "--------------------------------------------------" << endl;

        battle_loop(player_slime, enemy_slime);
    }

    //��Ҹ�����ʾ��Ϣѡ����Ҫʵʩ�ļ���
    Skill player_choose_skill(Slime& player_slime) {
        int choice_skill;
        cout << "Select the skill (1 for Tackle, 2 for " << player_slime.skills[1].name << "): ";
        while (true) {
            cin >> choice_skill;
            if (choice_skill == 1) {
                return player_slime.skills[0];
            } else if (choice_skill == 2) {
                return player_slime.skills[1];
            }
            cout << "Select the skill (1 for Tackle, 2 for " << player_slime.skills[1].name << "): ";
        }
    }

    //������ʾ��Ϣ�������ϵ�ʷ��ķ
    void changeslime(Slime*& player_slime) {
        int choice_slime;
        cout << "Select your next slime (";

        vector<int> aliveSlimes;
        for (int i = 0; i < player.size(); i++) {
            if (player[i].is_alive() && player[i].name != player_slime->name) {
                aliveSlimes.push_back(i + 1);
            }
        }

        for (size_t i = 0; i < aliveSlimes.size(); i++) {
            cout << aliveSlimes[i] << " for " << player[aliveSlimes[i] - 1].name;
            if (i < aliveSlimes.size() - 1) {
                cout << ", ";
            }
        }
        cout << "): ";

        while (true) {
            cin >> choice_slime;
            if (choice_slime >= 1 && choice_slime <= 3 && player[choice_slime - 1].is_alive() && player[choice_slime - 1].name != player_slime->name) {
                player_slime = &player[choice_slime - 1];
                break;
            }
            cout << "Select your next slime (";
            for (size_t i = 0; i < aliveSlimes.size(); i++) {
                cout << aliveSlimes[i] << " for " << player[aliveSlimes[i] - 1].name;
                if (i < aliveSlimes.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "): ";
        }
    }

    //������ʾ��Ϣѡ����Ҫ���ж�
    int player_turn(Slime& player_slime, Slime& enemy_slime) {
        int choice_action;
        int remain_slimes = 0;
        for (auto& slime : player) {
            if (slime.is_alive()) {
                remain_slimes++;
            }
        }
        if (remain_slimes > 1) {
            cout << "Select your action (1 for skill, 2 for change): ";
        } else {
            cout << "Select your action (1 for skill): ";
        }
        while (true) {
            cin >> choice_action;
            if (choice_action == 1) {
                return 1;
            } else if (choice_action == 2 && remain_slimes > 1) {
                return 2;
            }
            cout << "Select your action (1 for skill" << (remain_slimes > 1 ? ", 2 for change" : "") << "): ";
        }
    }

    //�з�ѡ����Ӧ���ж�
    bool enemy_turn(Slime& enemy_slime, Slime& player_slime) {
        string action = enemy_strategy.choose_action(player_slime, enemy_slime);
        if (action == "skill_1") {
            enemy_slime.use_skill(enemy_slime.skills[0], player_slime);
        } else {
            enemy_slime.use_skill(enemy_slime.skills[1], player_slime);
        }
        return player_slime.is_alive();
    }

    //ÿ�ֽ��������Ƿ�ֳ�ʤ��
    bool check_winner() {
        bool player_alive = false;
        bool enemy_alive = false;

        for (auto& slime : player) {
            if (slime.is_alive()) {
                player_alive = true;
                break;
            }
        }
        for (auto& slime : enemy) {
            if (slime.is_alive()) {
                enemy_alive = true;
                break;
            }
        }
        if (!enemy_alive||!player_alive) {
            return true;
        }
        return false;
    }

    void battle_loop(Slime*& player_slime, Slime*& enemy_slime) {
        while (round <= 100) {
            cout << "Round " << round << endl;

            int player_priority = 0;
            int enemy_priority = 0;

            // �׶�һ��ѡ���ж�
            //�����ǵз��ж�(�з�ֻ����ݱ��غϿ�ʼʱ��ҳ��ϵ�ʷ��ķ�����о���)
            string enemy_action = enemy_strategy.choose_action(*player_slime, *enemy_slime);
            if (enemy_action == "skill_1") {
                enemy_priority = enemy_slime->skills[0].priority;
            } else {
                enemy_priority = enemy_slime->skills[1].priority;
            }

            //Ȼ�������ѡ��
            Skill slime_skill = enemy_slime->skills[0];
            int player_action_choice = 1;
            if (player_slime->is_alive()) {
                player_action_choice = player_turn(*player_slime, *enemy_slime);
                if (player_action_choice == 2) {
                    player_priority = 6;
                } else {
                    slime_skill = player_choose_skill(*player_slime);
                }
            }

            // �׶ζ���ʵʩ�ж������ȼ����ߵ����ȷ�����
            //���˫����ѡ��ʹ�ü����Ҽ��ܵ����ȼ���ͬ����ôʷ��ķ���ٶ���ֵ���ߵ�һ���ȷ�������
            if ((player_priority == enemy_priority && player_slime->speed > enemy_slime->speed) || (player_priority > enemy_priority)) {
                if (player_action_choice == 1) {
                    int damage = player_slime->use_skill(slime_skill, *enemy_slime);
                    cout << "Your " << player_slime->name << " uses " << slime_skill.name << "! Damage: " << damage << endl;
                } else {
                    changeslime(player_slime);
                    cout << "You send " << player_slime->name << endl;
                }
                if (enemy_slime->is_alive() && enemy_action == "skill_1") {
                    int damage = enemy_slime->use_skill(enemy_slime->skills[0], *player_slime);
                    cout << "Enemy's " << enemy_slime->name << " uses " << enemy_slime->skills[0].name << "! Damage: " << damage << endl;
                } else if(enemy_slime->is_alive() && enemy_action == "skill_2") {
                    int damage = enemy_slime->use_skill(enemy_slime->skills[1], *player_slime);
                    cout << "Enemy's " << enemy_slime->name << " uses " << enemy_slime->skills[1].name << "! Damage: " << damage << endl;
                }
            //������ܵ����ȼ���ʷ��ķ���ٶȶ���ͬ����ô�з��ȷ������ܡ�
            } else if ((player_priority == enemy_priority && player_slime->speed <= enemy_slime->speed) || (player_priority < enemy_priority)) {
                if (enemy_action == "skill_1") {
                    int damage = enemy_slime->use_skill(enemy_slime->skills[0], *player_slime);
                    cout << "Enemy's " << enemy_slime->name << " uses " << enemy_slime->skills[0].name << "! Damage: " << damage << endl;
                } else {
                    int damage = enemy_slime->use_skill(enemy_slime->skills[1], *player_slime);
                    cout << "Enemy's " << enemy_slime->name << " uses " << enemy_slime->skills[1].name << "! Damage: " << damage << endl;
                }
                if (player_slime->is_alive() && player_action_choice == 1) {
                    int damage = player_slime->use_skill(slime_skill, *enemy_slime);
                    cout << "Your " << player_slime->name << " uses " << slime_skill.name << "! Damage: " << damage << endl;
                }
            }

            bool flag = check_winner();

            // �׶�������������ʷ��ķ
            if (!player_slime->is_alive()) {
                cout << "Your " << player_slime->name << " is beaten" << endl;
                if(flag){
                    cout << "You Lose" << endl;
                    return;
                }
                if (!flag) {
                    changeslime(player_slime);
                    cout << "You send " << player_slime->name << endl;
                }
            }
            if (!enemy_slime->is_alive()) {
                cout << "Enemy's " << enemy_slime->name << " is beaten" << endl;
                if(flag){
                    cout << "You Win" << endl;
                    return;
                }
                if (!flag) {
                    enemy_slime = enemy_strategy.choose_next_slime(player_slime, enemy);
                    cout << "Enemy sends " << enemy_slime->name << endl;
                }
            }
            //����ڵ� 100 �غϵĽ����׶���δ�ֳ�ʤ������ô˫��ƽ��
            if(round == 100){
                cout << "Draw" << endl;
                return;
            }
            //δ�ֳ�ʤ������ʾ˫��ʷ��ķʣ��HP
            cout << "Your " << player_slime->name << ": HP " << player_slime->current_HP << " || Enemy's " << enemy_slime->name << ": HP " << enemy_slime->current_HP << endl;
            cout << "--------------------------------------------------" << endl;
            round++;
        }
    }
};

int main() {
    Skill Tackle("Tackle","Attack","Ordinary",20,1,0);
    Skill Leaf("Leaf","Attack","Grass",20,1,0);
    Skill Flame("Flame","Attack","Fire",20,1,0);
    Skill Stream("Stream","Attack","Water",20,1,0);
    Slime Green("Green","Grass",110,110,10,10,10,Tackle,Leaf);
    Slime Red("Red","Fire",100,100,11,10,11,Tackle,Flame);
    Slime Blue("Blue","Water",100,100,10,11,9,Tackle,Stream);

    vector<Slime>player = {Green,Red,Blue};
    vector<Slime>enemy = {Green,Red,Blue};

    SimpleEnemy enemy_strategy;
    Battle battle(player, enemy);
    battle.start_battle();

    return 0;
}

