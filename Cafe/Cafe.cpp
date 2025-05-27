#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

using namespace std;

struct State {
    long long cost; // длинная стоимость
    int prev_day;   // предыдущий день
    int prev_c;     // количество купонов в предыдущем состоянии
    bool used_coupon; // использовали ли купон в этом шаге
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> costs(N);
    for (int i = 0; i < N; ++i) {
        cin >> costs[i];
    }

    // Максимальное число купонов - не больше N+1
    int max_coupons = N + 1;

    // Инициализация DP таблицы
    vector<vector<State>> dp(N + 1, vector<State>(max_coupons, { LLONG_MAX, -1, -1, false }));

    // Начальное состояние: 0 дней, 0 купонов, стоимость 0
    dp[0][0].cost = 0;

    for (int i = 1; i <= N; ++i) {
        int c_cost = costs[i - 1];
        for (int c = 0; c < max_coupons; ++c) {
            if (dp[i - 1][c].cost == LLONG_MAX)
                continue;

            long long prev_cost = dp[i - 1][c].cost;

            // Вариант 1: платим за обед
            int new_coupons = c + (c_cost > 100 ? 1 : 0);
            if (new_coupons < max_coupons) {
                long long new_cost = prev_cost + c_cost;
                if (new_cost < dp[i][new_coupons].cost) {
                    dp[i][new_coupons] = { new_cost, i - 1, c, false };
                }
            }

            // Вариант 2: используем купон (если есть)
            if (c > 0) {
                long long new_cost = prev_cost;
                if (new_cost < dp[i][c - 1].cost) {
                    dp[i][c - 1] = { new_cost, i - 1, c, true };
                }
            }
        }
    }

    // Находим лучший результат по минимальной стоимости,
    // при равенстве — по максимальному количеству оставшихся купонов
    long long min_total_cost = LLONG_MAX;
    int best_c = -1;
    for (int c = 0; c < max_coupons; ++c) {
        if (dp[N][c].cost < min_total_cost ||
            (dp[N][c].cost == min_total_cost && c > best_c)) {
            min_total_cost = dp[N][c].cost;
            best_c = c;
        }
    }

    // Восстановление путей использования купонов
    vector<int> used_days;
    int cur_day = N;
    int cur_c = best_c;

    while (cur_day > 0) {
        State& st = dp[cur_day][cur_c];
        if (st.used_coupon) {
            used_days.push_back(cur_day);
        }
        int prev_day = st.prev_day;
        int prev_c = st.prev_c;
        cur_day = prev_day;
        cur_c = prev_c;
    }

    // Вывод результатов
    cout << min_total_cost << "\n";
    cout << best_c << " " << used_days.size() << "\n";

    sort(used_days.begin(), used_days.end());
    for (int day : used_days) {
        cout << day << "\n";
    }

    return 0;
}