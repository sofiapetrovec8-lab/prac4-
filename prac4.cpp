#include <iostream>
using namespace std;

const int MAX_N = 10000;

struct Stats {
    long long comparisons;
    long long moves;
    int steps;
};

// Копіювання початкового масиву
void copyArray(const int source[], int destinatiKOTpracon[], int n) {
    for (int i = 0; i < n; i++) {
        destination[i] = source[i];
    }
}

void printArray(const int a[], int n) {
    for (int i = 0; i < n; i++) {
        cout << a[i] << " ";
    }
    cout << endl;
}

// Обчислення суми елементів
long long getSum(const int a[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += a[i];
    }
    return sum;
}

// Обчислення суми квадратів елементів
long long getSumSquares(const int a[], int n) {
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += 1LL * a[i] * a[i];
    }
    return sum;
}

// Перевірка впорядкованості масиву
bool isSorted(const int a[], int n) {
    for (int i = 1; i < n; i++) {
        if (a[i - 1] > a[i]) {
            return false;
        }
    }
    return true;
}

// Перевірка збереження складу масиву
bool checkContent(const int before[], const int after[], int n) {
    if (getSum(before, n) != getSum(after, n)) {
        return false;
    }

    if (getSumSquares(before, n) != getSumSquares(after, n)) {
        return false;
    }

    return true;
}

// Формування кроків послідовним діленням навпіл
void makeHalvingGaps(int n, int gaps[], int& count) {
    count = 0;

    if (n <= 1) {
        gaps[count++] = 1;
        return;
    }

    int gap = n / 2;

    while (gap > 0) {
        gaps[count++] = gap;
        gap /= 2;
    }
}

// Формування послідовності кроків 3h + 1
void makeKnuthGaps(int n, int gaps[], int& count) {
    int temp[MAX_N];
    int tempCount = 0;
    int h = 1;

    while (h < n) {
        temp[tempCount++] = h;
        h = 3 * h + 1;
    }

    count = 0;

    for (int i = tempCount - 1; i >= 0; i--) {
        gaps[count++] = temp[i];
    }

    if (count == 0) {
        gaps[count++] = 1;
    }
}

// Власна послідовність: h = 3*h - 1
void makeOwnGaps(int n, int gaps[], int& count) {
    int temp[MAX_N];
    int tempCount = 0;
    int h = 1;

    while (h < n) {
        temp[tempCount++] = h;
        h = 3 * h - 1;
    }

    count = 0;

    for (int i = tempCount - 1; i >= 0; i--) {
        gaps[count++] = temp[i];
    }

    if (count == 0) {
        gaps[count++] = 1;
    }
}

// Перевірка коректності послідовності кроків
bool checkGaps(const int gaps[], int count, int n) {
    if (count <= 0) {
        return false;
    }

    if (gaps[count - 1] != 1) {
        return false;
    }

    if (n > 1 && gaps[0] >= n) {
        return false;
    }

    for (int i = 0; i < count; i++) {
        if (gaps[i] <= 0) {
            return false;
        }
    }

    return true;
}

// Звичайне сортування вставками
Stats insertionSort(int a[], int n) {
    Stats s = { 0, 0, 1 };

    for (int i = 1; i < n; i++) {
        int x = a[i];
        s.moves++;
        int j = i - 1;

        while (j >= 0) {
            s.comparisons++;

            if (a[j] > x) {
                a[j + 1] = a[j];
                s.moves++;
                j--;
            }
            else {
                break;
            }
        }

        a[j + 1] = x;
        s.moves++;
    }

    return s;
}

// Сортування Шелла
Stats shellSort(int a[], int n, const int gaps[], int gapCount, bool showSteps) {
    Stats s = { 0, 0, gapCount };

    for (int g = 0; g < gapCount; g++) {
        int gap = gaps[g];

        for (int i = gap; i < n; i++) {
            int temp = a[i];
            s.moves++;
            int j = i;

            while (j >= gap) {
                s.comparisons++;

                if (a[j - gap] > temp) {
                    a[j] = a[j - gap];
                    s.moves++;
                    j -= gap;
                }
                else {
                    break;
                }
            }

            a[j] = temp;
            s.moves++;
        }

        if (showSteps) {
            cout << "After gap " << gap << ": ";
            printArray(a, n);
        }
    }

    return s;
}

void printGaps(const int gaps[], int count) {
    for (int i = 0; i < count; i++) {
        cout << gaps[i];
        if (i + 1 < count) {
            cout << " -> ";
        }
    }
    cout << endl;
}

// Демонстрація контрольного прикладу
void controlExample() {
    int a[] = { 12, 5, 9, 1, 8, 3, 10, 2 };
    int n = 8;
    int gaps[20];
    int gapCount;

    makeHalvingGaps(n, gaps, gapCount);

    cout << "CONTROL EXAMPLE" << endl;
    cout << "Original array: ";
    printArray(a, n);
    cout << "Gaps: ";
    printGaps(gaps, gapCount);

    Stats s = shellSort(a, n, gaps, gapCount, true);

    cout << "Comparisons: " << s.comparisons << endl;
    cout << "Moves: " << s.moves << endl;
    cout << "Steps: " << s.steps << endl;
    cout << "Check: " << (isSorted(a, n) ? "OK" : "ERROR") << endl;
    cout << endl;
}

bool runOneTest(const int input[], const int expected[], int n, const char* sequenceName,
    bool useKnuth, bool printError) {
    int before[MAX_N];
    int work[MAX_N];
    int gaps[MAX_N];
    int gapCount;

    copyArray(input, before, n);
    copyArray(input, work, n);

    if (useKnuth) {
        makeKnuthGaps(n, gaps, gapCount);
    }
    else {
        makeHalvingGaps(n, gaps, gapCount);
    }

    Stats s = shellSort(work, n, gaps, gapCount, false);

    bool ok = true;

    if (!checkGaps(gaps, gapCount, n)) {
        ok = false;
    }

    if (!isSorted(work, n)) {
        ok = false;
    }

    for (int i = 0; i < n; i++) {
        if (work[i] != expected[i]) {
            ok = false;
        }
    }

    if (!checkContent(before, work, n)) {
        ok = false;
    }

    if (printError && !ok) {
        cout << sequenceName << ": resulting array: ";
        printArray(work, n);
    }

    return ok;
}

// Автоматизовані тести
void automaticTests() {
    const int TESTS = 12;
    const int MAX_TEST = 20;

    int tests[TESTS][MAX_TEST] = {};
    int expected[TESTS][MAX_TEST] = {};
    int sizes[TESTS] = { 0, 1, 5, 5, 5, 5, 5, 4, 2, 6, 9, 20 };

    tests[1][0] = 4;
    expected[1][0] = 4;

    for (int i = 0; i < 5; i++) {
        tests[2][i] = i + 1;
        expected[2][i] = i + 1;
        tests[3][i] = 5 - i;
        expected[3][i] = i + 1;
    }

    int t5[] = { 7, 2, 9, 1, 5 };
    int e5[] = { 1, 2, 5, 7, 9 };
    int t6[] = { 3, 1, 3, 2, 1 };
    int e6[] = { 1, 1, 2, 3, 3 };
    int t7[] = { 0, -6, 8, -2, 4 };
    int e7[] = { -6, -2, 0, 4, 8 };
    int t8[] = { 6, 6, 6, 6 };
    int e8[] = { 6, 6, 6, 6 };
    int t9[] = { 2, 1 };
    int e9[] = { 1, 2 };
    int t10[] = { 1, 2, 4, 3, 5, 6 };
    int e10[] = { 1, 2, 3, 4, 5, 6 };
    int t11[] = { 9, 1, 8, 2, 7, 3, 6, 4, 5 };
    int e11[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int t12[] = { 12, 3, 19, 1, 15, 7, 20, 4, 9, 2, 18, 6, 11, 5, 17, 8, 14, 10, 16, 13 };
    int e12[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

    for (int i = 0; i < 5; i++) {
        tests[4][i] = t5[i]; expected[4][i] = e5[i];
        tests[5][i] = t6[i]; expected[5][i] = e6[i];
        tests[6][i] = t7[i]; expected[6][i] = e7[i];
    }
    for (int i = 0; i < 4; i++) {
        tests[7][i] = t8[i]; expected[7][i] = e8[i];
    }
    for (int i = 0; i < 2; i++) {
        tests[8][i] = t9[i]; expected[8][i] = e9[i];
    }
    for (int i = 0; i < 6; i++) {
        tests[9][i] = t10[i]; expected[9][i] = e10[i];
    }
    for (int i = 0; i < 9; i++) {
        tests[10][i] = t11[i]; expected[10][i] = e11[i];
    }
    for (int i = 0; i < 20; i++) {
        tests[11][i] = t12[i]; expected[11][i] = e12[i];
    }

    cout << "AUTOMATED TESTS" << endl;
    int success = 0;

    for (int i = 0; i < TESTS; i++) {
        bool ok1 = runOneTest(tests[i], expected[i], sizes[i],
            "Halving", false, true);
        bool ok2 = runOneTest(tests[i], expected[i], sizes[i],
            "3h + 1", true, true);

        cout << "Test " << i + 1 << ": "
            << (ok1 ? "Halving - OK" : "Halving - ERROR")
            << ", "
            << (ok2 ? "3h + 1 - OK" : "3h + 1 - ERROR")
            << endl;

        if (ok1) success++;
        if (ok2) success++;
    }

    cout << "Total successful checks: " << success << " out of 24" << endl;
    cout << endl;
}

void fillAscending(int a[], int n) {
    for (int i = 0; i < n; i++) {
        a[i] = i + 1;
    }
}

void fillDescending(int a[], int n) {
    for (int i = 0; i < n; i++) {
        a[i] = n - i;
    }
}

void fillAlmostSorted(int a[], int n) {
    fillAscending(a, n);

    if (n >= 4) {
        int temp = a[0];
        a[0] = a[1];
        a[1] = temp;

        temp = a[n / 2];
        a[n / 2] = a[n / 2 + 1];
        a[n / 2 + 1] = temp;

        temp = a[n - 2];
        a[n - 2] = a[n - 1];
        a[n - 1] = temp;
    }
}

void fillMixed(int a[], int n) {
    for (int i = 0; i < n; i++) {
        a[i] = ((i * 37 + 11) % n) + 1;
    }
}

// Проведення експерименту для одного набору даних
void experiment(const char* type, int n, int kind) {
    int* original = new int[n];
    int* work = new int[n];
    int gaps1[MAX_N];
    int gaps2[MAX_N];
    int count1, count2;

    if (kind == 1) {
        fillAscending(original, n);
    }
    else if (kind == 2) {
        fillDescending(original, n);
    }
    else if (kind == 3) {
        fillAlmostSorted(original, n);
    }
    else {
        fillMixed(original, n);
    }

    makeHalvingGaps(n, gaps1, count1);
    makeKnuthGaps(n, gaps2, count2);

    copyArray(original, work, n);
    Stats insertion = insertionSort(work, n);

    copyArray(original, work, n);
    Stats shell1 = shellSort(work, n, gaps1, count1, false);

    copyArray(original, work, n);
    Stats shell2 = shellSort(work, n, gaps2, count2, false);

    cout << n << " | " << type
        << " | " << insertion.comparisons << "/" << insertion.moves
        << " | " << shell1.comparisons << "/" << shell1.moves << "/" << shell1.steps
        << " | " << shell2.comparisons << "/" << shell2.moves << "/" << shell2.steps
        << endl;

    delete[] original;
    delete[] work;
}

// Обробка індивідуального варіанта 18
void individualVariant() {
    int original[] = { 36, 12, 24, 40, 9, 29, 16, 33, 11, 20, 37, 8, 27, 14, 22 };
    int n = 15;
    int a[15];
    int gaps1[20], gaps2[20], gaps3[20];
    int count1, count2, count3;

    makeHalvingGaps(n, gaps1, count1);
    makeKnuthGaps(n, gaps2, count2);
    makeOwnGaps(n, gaps3, count3);

    cout << "INDIVIDUAL VARIANT NO. 18" << endl;
    cout << "Original array: ";
    printArray(original, n);

    copyArray(original, a, n);
    Stats insertion = insertionSort(a, n);
    cout << "Insertion Sort: ";
    printArray(a, n);
    cout << "Comparisons = " << insertion.comparisons
        << ", moves = " << insertion.moves << endl;

    copyArray(original, a, n);
    Stats shell1 = shellSort(a, n, gaps1, count1, false);
    cout << "Shell Sort, halving: ";
    printArray(a, n);
    cout << "Gaps: "; printGaps(gaps1, count1);
    cout << "Comparisons = " << shell1.comparisons
        << ", moves = " << shell1.moves << endl;

    copyArray(original, a, n);
    Stats shell2 = shellSort(a, n, gaps2, count2, false);
    cout << "Shell Sort, 3h + 1: ";
    printArray(a, n);
    cout << "Gaps: "; printGaps(gaps2, count2);
    cout << "Comparisons = " << shell2.comparisons
        << ", moves = " << shell2.moves << endl;

    copyArray(original, a, n);
    Stats shell3 = shellSort(a, n, gaps3, count3, false);
    cout << "Custom sequence 3h - 1: ";
    printArray(a, n);
    cout << "Gaps: "; printGaps(gaps3, count3);
    cout << "Comparisons = " << shell3.comparisons
        << ", moves = " << shell3.moves << endl;

    cout << endl;
}

// Введення та сортування власного масиву
void manualInput() {
    int n;

    cout << "Enter the number of elements (1-10000): ";
    cin >> n;

    if (n < 1 || n > MAX_N) {
        cout << "Invalid array size." << endl;
        return;
    }

    int* original = new int[n];
    int* work = new int[n];
    int gaps1[MAX_N], gaps2[MAX_N];
    int count1, count2;

    cout << "Enter " << n << " integers:" << endl;
    for (int i = 0; i < n; i++) {
        cin >> original[i];
    }

    makeHalvingGaps(n, gaps1, count1);
    makeKnuthGaps(n, gaps2, count2);

    copyArray(original, work, n);
    Stats s1 = shellSort(work, n, gaps1, count1, true);
    cout << "Halving: " << s1.comparisons << " comparisons, "
        << s1.moves << " moves" << endl;

    copyArray(original, work, n);
    Stats s2 = shellSort(work, n, gaps2, count2, true);
    cout << "3h + 1: " << s2.comparisons << " comparisons, "
        << s2.moves << " moves" << endl;

    delete[] original;
    delete[] work;
}

// Головна функція програми
int main() {
    int choice;

    cout << "SHELL SORT" << endl;
    cout << "1 - enter a custom array" << endl;
    cout << "2 - run variant 18 and all tests" << endl;
    cout << "Your choice: ";
    cin >> choice;
    cout << endl;

    if (choice == 1) {
        manualInput();
        return 0;
    }

    individualVariant();
    controlExample();
    automaticTests();

    cout << "LARGE EXPERIMENT" << endl;
    cout << "Format: n | dataset | Insertion C/M | Shell 1 C/M/S | Shell 2 C/M/S" << endl;
    cout << endl;

    int sizes[] = { 100, 1000, 10000 };

    for (int i = 0; i < 3; i++) {
        experiment("Sorted", sizes[i], 1);
        experiment("Reverse", sizes[i], 2);
        experiment("Nearly sorted", sizes[i], 3);
        experiment("Mixed", sizes[i], 4);
    }

    return 0;
}
