#include <iostream>
#include <pqxx/pqxx>
#include <iomanip>
using namespace std;
using namespace pqxx;

void printSeparator() {
    cout << "\n" << string(80, '=') << "\n\n";
}

int main() {
    try {
        // Подключение к БД
        connection conn("dbname=tanks_blitz user=ivanbekrenev password=123 hostaddr=127.0.0.1 port=5432");
        if (conn.is_open()) {
            cout << "+ Подключение к БД tanks_blitz успешно!\n";
        } else {
            cerr << "- Ошибка подключения к БД\n";
            return 1;
        }

        work txn(conn);

        // ЗАПРОС 1: Все игроки с сортировкой (ORDER BY)
        cout << "\n=== ЗАПРОС 1: Все игроки (сортировка по количеству боёв) ===\n";
        result r1 = txn.exec(
            "SELECT username, total_battles, win_rate, average_damage "
            "FROM players "
            "ORDER BY total_battles DESC "
            "LIMIT 5"
        );
        cout << left << setw(35) << "Игрок" << setw(16) << "Боёв" 
             << setw(12) << "WR %" << "Ср.урон\n";
        cout << string(65, '-') << "\n";
        for (auto row : r1) {
            cout << left << setw(30) << row[0].c_str()
                 << setw(12) << row[1].as<int>()
                 << setw(12) << fixed << setprecision(2) << row[2].as<double>()
                 << row[3].as<int>() << "\n";
        }
        printSeparator();

        // ЗАПРОС 2: INNER JOIN - детали последнего боя
        cout << "=== ЗАПРОС 2: Детали последнего боя (INNER JOIN) ===\n";
        result r2 = txn.exec(
            "SELECT p.username, t.tank_name, t.tier, bp.team, bp.damage_dealt, bp.kills, bp.survived "
            "FROM battle_participants bp "
            "INNER JOIN players p ON bp.player_id = p.player_id "
            "INNER JOIN tanks t ON bp.tank_id = t.tank_id "
            "INNER JOIN battles b ON bp.battle_id = b.battle_id "
            "WHERE b.battle_id = 1 "
            "ORDER BY bp.team DESC, bp.damage_dealt DESC"
        );
        cout << left << setw(35) << "Игрок" << setw(35) << "Танк" 
             << setw(10) << "Урон" << setw(13) << "Фрагов" << setw(10) << " Статус\n";
        cout << string(90, '-') << "\n";
        for (auto row : r2) {
            cout << left << setw(30) << row[0].c_str()
                 << setw(30) << (string(row[1].c_str()))
                 << setw(10) << row[4].as<int>()
                 << setw(10) << row[5].as<int>()
                 << (row[6].as<bool>() ? "Выжил" : "Уничтожен") << "\n";
        }
        printSeparator();

        // ЗАПРОС 3: Агрегатные функции - статистика по командам
        cout << "=== ЗАПРОС 3: Статистика команд (SUM, AVG, COUNT) ===\n";
        result r3 = txn.exec(
            "SELECT team, "
            "COUNT(*) as players_count, "
            "SUM(damage_dealt) as total_damage, "
            "ROUND(AVG(damage_dealt)) as avg_damage, "
            "SUM(kills) as total_kills "
            "FROM battle_participants "
            "WHERE battle_id = 1 "
            "GROUP BY team "
            "ORDER BY total_damage DESC"
        );
        cout << left << setw(22) << "Команда" << setw(15) << "Игроков" 
             << setw(35) << "Всего урона" << setw(20) << "Ср.урон" << "Фрагов\n";
        cout << string(80, '-') << "\n";
        for (auto row : r3) {
            cout << left << setw(25) << (string(row[0].c_str()) == "allies" ? "Союзники" : "Противники")
                 << setw(10) << row[1].as<int>()
                 << setw(18) << row[2].as<int>()
                 << setw(15) << row[3].as<int>()
                 << row[4].as<int>() << "\n";
        }
        printSeparator();

        // ЗАПРОС 4: GROUP BY + HAVING - топ танков с минимумом боёв
        cout << "=== ЗАПРОС 4: Топ танков по фрагам (GROUP BY + HAVING) ===\n";
        result r4 = txn.exec(
            "SELECT t.tank_name, t.nation, "
            "SUM(bp.kills) as total_kills, "
            "COUNT(bp.participant_id) as battles "
            "FROM tanks t "
            "INNER JOIN battle_participants bp ON t.tank_id = bp.tank_id "
            "GROUP BY t.tank_id, t.tank_name, t.nation "
            "HAVING SUM(bp.kills) > 0 "
            "ORDER BY total_kills DESC "
            "LIMIT 5"
        );
        cout << left << setw(35) << "Танк" << setw(25) << "Нация" 
             << setw(15) << "Фрагов" << "Боёв\n";
        cout << string(80, '-') << "\n";
        for (auto row : r4) {
            cout << left << setw(30) << row[0].c_str()
                 << setw(35) << row[1].c_str()
                 << setw(12) << row[2].as<int>()
                 << row[3].as<int>() << "\n";
        }
        printSeparator();

        // ЗАПРОС 5: LEFT JOIN - все танки и их использование
        cout << "=== ЗАПРОС 5: Использование танков (LEFT JOIN) ===\n";
        result r5 = txn.exec(
            "SELECT t.tank_name, t.tier, t.tank_type, "
            "COALESCE(COUNT(pt.player_tank_id), 0) as owners "
            "FROM tanks t "
            "LEFT JOIN player_tanks pt ON t.tank_id = pt.tank_id "
            "GROUP BY t.tank_id, t.tank_name, t.tier, t.tank_type "
            "ORDER BY owners DESC "
            "LIMIT 7"
        );
        cout << left << setw(35) << "Танк" << setw(20) << "Уровень" 
             << setw(15) << "Тип" << "Владельцев\n";
        cout << string(70, '-') << "\n";
        for (auto row : r5) {
            cout << left << setw(35) << row[0].c_str()
                 << setw(12) << row[1].as<int>()
                 << setw(12) << row[2].c_str()
                 << row[3].as<int>() << "\n";
        }
        printSeparator();

        // ЗАПРОС 6: WHERE - поиск игроков с высоким винрейтом
        cout << "=== ЗАПРОС 6: Игроки с винрейтом > 50% и кол-во боев > 10000 (WHERE) ===\n";
        result r6 = txn.exec(
            "SELECT username, total_battles, win_rate "
            "FROM players "
            "WHERE win_rate > 50.0 AND total_battles > 10000 "
            "ORDER BY win_rate DESC"
        );
        cout << left << setw(35) << "Игрок" << setw(15) << "Боёв" << "WR %\n";
        cout << string(55, '-') << "\n";
        for (auto row : r6) {
            cout << left << setw(35) << row[0].c_str()
                 << setw(15) << row[1].as<int>()
                 << fixed << setprecision(2) << row[2].as<double>() << "\n";
        }
        printSeparator();

        // ЗАПРОС 7: Подзапрос - лучший игрок по среднему урону
        cout << "=== ЗАПРОС 7: Топ игрок по урону в бою (подзапрос) ===\n";
        result r7 = txn.exec(
            "SELECT p.username, bp.damage_dealt, t.tank_name "
            "FROM battle_participants bp "
            "INNER JOIN players p ON bp.player_id = p.player_id "
            "INNER JOIN tanks t ON bp.tank_id = t.tank_id "
            "WHERE bp.damage_dealt = ("
            "    SELECT MAX(damage_dealt) FROM battle_participants WHERE battle_id = 1"
            ") AND bp.battle_id = 1"
        );
        for (auto row : r7) {
            cout << "Лучший игрок: " << row[0].c_str() << "\n"
                 << "Танк: " << row[2].c_str() << "\n"
                 << "Урон: " << row[1].as<int>() << "\n";
        }
        printSeparator();

        // ЗАПРОС 8: INSERT - добавить нового игрока
        cout << "=== ЗАПРОС 8: Добавление нового игрока (INSERT) ===\n";
        txn.exec(
            "INSERT INTO players (username, total_battles, win_rate, average_damage) "
            "VALUES ('TestPlayer_2025', 100, 48.50, 1500) "
            "ON CONFLICT (username) DO NOTHING"
        );
        cout << "✓ Игрок 'TestPlayer_2025' добавлен\n";
        printSeparator();

        // ЗАПРОС 9: UPDATE - обновить статистику игрока
        cout << "=== ЗАПРОС 9: Обновление статистики (UPDATE) ===\n";
        txn.exec(
            "UPDATE players "
            "SET total_battles = total_battles + 1, "
            "    win_rate = ROUND((total_battles * win_rate + 100) / (total_battles + 1), 2) "
            "WHERE username = 'M_rGardan'"
        );
        cout << "✓ Статистика игрока 'M_rGardan' обновлена\n";
        printSeparator();

        // ЗАПРОС 10: Статистика по нациям танков
        cout << "=== ЗАПРОС 10: Статистика по нациям (GROUP BY) ===\n";
        result r10 = txn.exec(
            "SELECT t.nation, "
            "COUNT(DISTINCT t.tank_id) as tanks_count, "
            "COALESCE(SUM(bp.damage_dealt), 0) as total_damage "
            "FROM tanks t "
            "LEFT JOIN battle_participants bp ON t.tank_id = bp.tank_id "
            "GROUP BY t.nation "
            "ORDER BY total_damage DESC"
        );
        cout << left << setw(25) << "Нация" << setw(18) << "Танков" << "Всего урона\n";
        cout << string(60, '-') << "\n";
        for (auto row : r10) {
            cout << left << setw(25) << row[0].c_str()
                 << setw(18) << row[1].as<int>()
                 << row[2].as<long>() << "\n";
        }
        printSeparator();

        txn.commit();
        cout << "✓ Все 10 запросов выполнены успешно!\n\n";

    } catch (const exception &e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
