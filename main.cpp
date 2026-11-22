#include "tubes.h"
#include <limits>

/* ===================== INPUT HELPERS ===================== */
string readLine(string prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

int readInt(string prompt) {
    int x;
    while (true) {
        cout << prompt;
        if (cin >> x) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return x;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "[!] Input harus berupa angka." << endl;
    }
}

string chooseRoomType() {
    int c;
    do {
        c = readInt("Tipe room (1=AC, 2=Non-AC): ");
    } while (c != 1 && c != 2);
    return (c == 1) ? "AC" : "Non-AC";
}

string chooseMethod() {
    int c;
    do {
        c = readInt("Metode (1=Cash, 2=Transfer): ");
    } while (c != 1 && c != 2);
    return (c == 1) ? "Cash" : "Transfer";
}

string chooseStatus() {
    int c;
    do {
        c = readInt("Status (1=Paid, 2=Late, 3=Unpaid): ");
    } while (c < 1 || c > 3);
    if (c == 1) return "Paid";
    if (c == 2) return "Late";
    return "Unpaid";
}

/* ===================== SAMPLE DATA & AUTO TEST ===================== */
void addRoom(ListParent &L, string id, string name, string type, int rent) {
    Room r = {id, name, type, rent};
    insertParent(L, createParent(r));
}

void addPayment(ListParent &L, string id, string period, int amt, string method, string status) {
    Payment p = {period, amt, method, status};
    insertChild(L, id, createChild(p));
}

void loadSampleData(ListParent &L) {
    addRoom(L, "R01", "Andi", "AC", 1200000);
    addRoom(L, "R02", "Budi", "Non-AC", 800000);
    addRoom(L, "R03", "Citra", "AC", 1300000);
    addRoom(L, "R04", "Dewi", "Non-AC", 850000);   // sengaja tanpa child

    addPayment(L, "R01", "01-2027", 1200000, "Transfer", "Paid");
    addPayment(L, "R01", "02-2027", 1200000, "Cash", "Paid");
    addPayment(L, "R02", "01-2027", 800000, "Cash", "Paid");
    addPayment(L, "R02", "02-2027", 400000, "Transfer", "Late");
    addPayment(L, "R03", "01-2027", 1300000, "Transfer", "Paid");
    addPayment(L, "R03", "02-2027", 0, "Cash", "Unpaid");
}

void runAutoTest() {
    ListParent L;
    createListParent(L);

    cout << "\n##### TEST 8a: List kosong #####" << endl;
    printAll(L);
    deleteParent(L, "R01");
    cout << "\n##### TEST 1: Tambah & tampil parent #####" << endl;
    loadSampleData(L);
    printParent(L);

    cout << "\n##### TEST 2: Cari parent #####" << endl;
    adrParent P = findParent(L, "R02");
    if (P != NULL) cout << "Ditemukan: " << P->info.roomID << " - " << P->info.tenantName << endl;
    if (findParent(L, "R99") == NULL) cout << "[!] Room R99 tidak ditemukan (benar)." << endl;

    cout << "\n##### TEST 3: Tambah child #####" << endl;
    addPayment(L, "R04", "01-2027", 850000, "Cash", "Paid");
    addPayment(L, "R99", "01-2027", 100, "Cash", "Paid");   // parent tidak ada

    cout << "\n##### TEST 4: Cari & tampil child per parent #####" << endl;
    printChild(findParent(L, "R01"));
    adrChild C = findChild(findParent(L, "R02"), "02-2027");
    if (C != NULL) cout << "Child ditemukan: " << C->info.period << " - " << C->info.status << endl;

    cout << "\n##### TEST 5: Hapus child #####" << endl;
    deleteChild(L, "R01", "01-2027");   // child pertama
    deleteChild(L, "R01", "12-2030");   // tidak ditemukan
    printChild(findParent(L, "R01"));

    cout << "\n##### TEST 6: Hapus parent #####" << endl;
    deleteParent(L, "R04");   // parent terakhir
    deleteParent(L, "R01");   // parent pertama
    deleteParent(L, "R77");   // tidak ditemukan

    cout << "\n##### TEST 7: Tampil seluruh MLL #####" << endl;
    printAll(L);

    cout << "\n##### EXTRA #####" << endl;
    printRoomsWithArrears(L);
    cout << "Pendapatan 01-2027: Rp" << monthlyIncome(L, "01-2027") << endl;
    adrParent top = roomWithMostPayments(L);
    if (top != NULL) cout << "Pembayaran terbanyak: " << top->info.roomID << " (" << countChild(top) << ")" << endl;
}

/* ===================== MENU ===================== */
void showMenu() {
    cout << "\n===== BOARDING HOUSE MANAGEMENT SYSTEM =====" << endl;
    cout << " 1. Tambah room" << endl;
    cout << " 2. Tampilkan semua room" << endl;
    cout << " 3. Cari room (by ID / nama tenant)" << endl;
    cout << " 4. Hapus room" << endl;
    cout << " 5. Tambah pembayaran pada room" << endl;
    cout << " 6. Tampilkan pembayaran suatu room" << endl;
    cout << " 7. Cari pembayaran (by periode)" << endl;
    cout << " 8. Hapus pembayaran" << endl;
    cout << " 9. Tampilkan seluruh data (printAll)" << endl;
    cout << "10. Total pembayaran per room" << endl;
    cout << "11. Room dengan tunggakan" << endl;
    cout << "12. Rekap pendapatan per bulan" << endl;
    cout << "13. Room dengan pembayaran terbanyak" << endl;
    cout << "14. Muat data contoh" << endl;
    cout << "15. Jalankan pengujian otomatis" << endl;
    cout << " 0. Keluar" << endl;
}

int main() {
    ListParent L;
    createListParent(L);
    int choice;

    do {
        showMenu();
        choice = readInt("Pilih menu: ");
        cout << endl;

        if (choice == 1) {
            Room r;
            r.roomID = readLine("Room ID: ");
            r.tenantName = readLine("Nama tenant: ");
            r.roomType = chooseRoomType();
            r.monthlyRent = readInt("Harga sewa/bulan: ");
            insertParent(L, createParent(r));
        } else if (choice == 2) {
            printParent(L);
        } else if (choice == 3) {
            int mode = readInt("Cari berdasarkan (1=ID, 2=Nama tenant): ");
            adrParent P = NULL;
            if (mode == 1) P = findParent(L, readLine("Room ID: "));
            else if (mode == 2) P = findParentByTenant(L, readLine("Nama tenant: "));
            else cout << "[!] Pilihan tidak valid." << endl;
            if (mode == 1 || mode == 2) {
                if (P == NULL) {
                    cout << "[!] Room tidak ditemukan." << endl;
                } else {
                    cout << "Ditemukan: " << P->info.roomID << " | " << P->info.tenantName
                         << " | " << P->info.roomType << " | Rp" << P->info.monthlyRent << endl;
                }
            }
        } else if (choice == 4) {
            deleteParent(L, readLine("Room ID yang dihapus: "));
        } else if (choice == 5) {
            string id = readLine("Room ID: ");
            if (findParent(L, id) == NULL) {
                cout << "[!] Room tidak ditemukan." << endl;
            } else {
                Payment p;
                p.period = readLine("Periode (MM-YYYY): ");
                p.amountPaid = readInt("Nominal bayar: ");
                p.method = chooseMethod();
                p.status = chooseStatus();
                insertChild(L, id, createChild(p));
            }
        } else if (choice == 6) {
            adrParent P = findParent(L, readLine("Room ID: "));
            if (P == NULL) cout << "[!] Room tidak ditemukan." << endl;
            else {
                cout << "Riwayat pembayaran room " << P->info.roomID << ":" << endl;
                printChild(P);
            }
        } else if (choice == 7) {
            adrParent P = findParent(L, readLine("Room ID: "));
            if (P == NULL) {
                cout << "[!] Room tidak ditemukan." << endl;
            } else {
                adrChild C = findChild(P, readLine("Periode (MM-YYYY): "));
                if (C == NULL) cout << "[!] Pembayaran tidak ditemukan." << endl;
                else cout << "Ditemukan: " << C->info.period << " | Rp" << C->info.amountPaid
                          << " | " << C->info.method << " | " << C->info.status << endl;
            }
        } else if (choice == 8) {
            string id = readLine("Room ID: ");
            string per = readLine("Periode (MM-YYYY): ");
            deleteChild(L, id, per);
        } else if (choice == 9) {
            printAll(L);
        } else if (choice == 10) {
            adrParent P = findParent(L, readLine("Room ID: "));
            if (P == NULL) cout << "[!] Room tidak ditemukan." << endl;
            else cout << "Total pembayaran room " << P->info.roomID << ": Rp" << totalPaid(P)
                      << " (" << countChild(P) << " transaksi)" << endl;
        } else if (choice == 11) {
            printRoomsWithArrears(L);
        } else if (choice == 12) {
            string per = readLine("Periode (MM-YYYY): ");
            cout << "Total pendapatan periode " << per << ": Rp" << monthlyIncome(L, per) << endl;
        } else if (choice == 13) {
            adrParent P = roomWithMostPayments(L);
            if (P == NULL) cout << "[!] Belum ada data room." << endl;
            else cout << "Room dengan pembayaran terbanyak: " << P->info.roomID << " ("
                      << P->info.tenantName << ") - " << countChild(P) << " transaksi" << endl;
        } else if (choice == 14) {
            loadSampleData(L);
        } else if (choice == 15) {
            runAutoTest();
        } else if (choice != 0) {
            cout << "[!] Menu tidak valid." << endl;
        }
    } while (choice != 0);

    cout << "Program selesai." << endl;
    return 0;
}
