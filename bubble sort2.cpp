#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace chrono;

// Lebar kolom tabel (diatur di satu tempat supaya selalu rapi)
const int W_METODE = 15;
const int W_STATUS = 17;
const int W_POSISI = 8;
const int W_WAKTU  = 12;
const int W_ITER   = 9;
const int W_BAND   = 12;
const int W_TOTAL  = W_METODE + W_STATUS + W_POSISI + W_WAKTU + W_ITER + W_BAND;

struct Result {
    int    posisi;
    int    iterasi;
    int    perbandingan;
    double waktu;
};

// ---------------- Sorting ----------------
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(arr[j], arr[j + 1]);
}

// ---------------- Sequential Search ----------------
Result sequentialSearch(const vector<int>& arr, int key) {
    Result r = {-1, 0, 0, 0};
    auto mulai = high_resolution_clock::now();

    for (int i = 0; i < (int)arr.size(); i++) {
        r.iterasi++;
        r.perbandingan++;
        if (arr[i] == key) { r.posisi = i; break; }
    }

    r.waktu = duration<double, milli>(high_resolution_clock::now() - mulai).count();
    return r;
}

// ---------------- Binary Search ----------------
Result binarySearch(const vector<int>& arr, int key) {
    Result r = {-1, 0, 0, 0};
    int low = 0, high = arr.size() - 1;
    auto mulai = high_resolution_clock::now();

    while (low <= high) {
        r.iterasi++;
        int mid = low + (high - low) / 2;

        r.perbandingan++;
        if (arr[mid] == key) { r.posisi = mid; break; }

        r.perbandingan++;
        if (key < arr[mid]) high = mid - 1;
        else                low  = mid + 1;
    }

    r.waktu = duration<double, milli>(high_resolution_clock::now() - mulai).count();
    return r;
}

// ---------------- Interpolation Search ----------------
Result interpolationSearch(const vector<int>& arr, int key) {
    Result r = {-1, 0, 0, 0};
    int low = 0, high = arr.size() - 1;
    auto mulai = high_resolution_clock::now();

    while (low <= high) {
        r.perbandingan++;
        if (key < arr[low]) break;

        r.perbandingan++;
        if (key > arr[high]) break;

        r.iterasi++;

        r.perbandingan++;                       // menghitung arr[low] == arr[high]
        if (arr[low] == arr[high]) {
            r.perbandingan++;
            if (arr[low] == key) r.posisi = low;
            break;
        }

        int pos = low + (double)(key - arr[low]) / (arr[high] - arr[low]) * (high - low);

        r.perbandingan++;
        if (arr[pos] == key) { r.posisi = pos; break; }

        r.perbandingan++;
        if (arr[pos] < key) low  = pos + 1;
        else                high = pos - 1;
    }

    r.waktu = duration<double, milli>(high_resolution_clock::now() - mulai).count();
    return r;
}

// ---------------- searchValue: pilih algoritma via string ----------------
Result searchValue(const vector<int>& arr, int key, const string& metode) {
    if (metode == "seq") return sequentialSearch(arr, key);
    if (metode == "bin") return binarySearch(arr, key);
    if (metode == "int") return interpolationSearch(arr, key);
    return {-1, 0, 0, 0};
}

// ---------------- Tampilan ----------------
void garis(char c = '-') {
    cout << string(W_TOTAL, c) << "\n";
}

void headerTabel() {
    cout << left
         << setw(W_METODE) << "Metode"
         << setw(W_STATUS) << "Status"
         << setw(W_POSISI) << "Posisi"
         << setw(W_WAKTU)  << "Waktu(ms)"
         << setw(W_ITER)   << "Iterasi"
         << setw(W_BAND)   << "Perbandingan" << "\n";
}

void tampilkanHasil(const string& nama, const Result& r) {
    bool ada = (r.posisi != -1);
    cout << left
         << setw(W_METODE) << nama
         << setw(W_STATUS) << (ada ? "Ditemukan" : "Tidak Ditemukan")
         << setw(W_POSISI) << (ada ? to_string(r.posisi) : "-")
         << setw(W_WAKTU)  << fixed << setprecision(3) << r.waktu
         << setw(W_ITER)   << r.iterasi
         << setw(W_BAND)   << r.perbandingan << "\n";
}

// ---------------- Main ----------------
int main() {
    const int N = 1000;
    vector<int> data(N);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distribusi(1, 3000);
    for (int i = 0; i < N; i++) data[i] = distribusi(gen);

    vector<int> dataUrut = data;
    bubbleSort(dataUrut);               // 1 metode sorting untuk Binary & Interpolation

    cout << "Masukkan nilai yang ingin dicari (1-3000): ";
    int key;
    cin >> key;

    Result seq   = searchValue(dataUrut, key, "seq");
    Result bin   = searchValue(dataUrut, key, "bin");
    Result inter = searchValue(dataUrut, key, "int");

    cout << "\nHasil Pencarian (n = " << N << " elemen, range 1-3000, sorting: Bubble Sort)\n";
    garis();
    headerTabel();
    garis();
    tampilkanHasil("Sequential",    seq);
    tampilkanHasil("Binary",        bin);
    tampilkanHasil("Interpolation", inter);
    garis();

    cout << "\n=== Program Selesai ===\n";
    return 0;
}
