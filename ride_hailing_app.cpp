#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <limits>
using namespace std;

class Pengguna {
private:
    string id_pengguna;
    string nama;
public:
    Pengguna(string id_pengguna, string nama) {
        this->id_pengguna = id_pengguna;
        this->nama = nama;
    }
    string getIdPengguna() { return id_pengguna; }
    string getNama() { return nama; }
};

class Driver {
protected:
    string id_driver;
    string nama_driver;
    string tipe_kendaraan;
    double tarif_per_km;
public:
    Driver(string id_driver, string nama_driver, string tipe_kendaraan, double tarif_per_km) {
        this->id_driver = id_driver;
        this->nama_driver = nama_driver;
        this->tipe_kendaraan = tipe_kendaraan;
        this->tarif_per_km = tarif_per_km;
    }
    virtual double hitungtarif(double jarak_tempuh) {
        return jarak_tempuh * tarif_per_km;
    }
    string getIdDriver() { return id_driver; }
    string getNamaDriver() { return nama_driver; }
    double getTarifPerKm() { return tarif_per_km; }
};

class PremiumDriver : public Driver {
public:
    PremiumDriver(string id_driver, string nama_driver, string tipe_kendaraan, double tarif_per_km)
        : Driver(id_driver, nama_driver, tipe_kendaraan, tarif_per_km) {}
    double hitungtarif(double jarak_tempuh) override {
        return jarak_tempuh * tarif_per_km * 1.2;
    }
};

class Kendaraan {
private:
    string plat_nomor;
    string merk;
    int tahun;
public:
    Kendaraan(string plat_nomor, string merk, int tahun) {
        this->plat_nomor = plat_nomor;
        this->merk = merk;
        this->tahun = tahun;
    }
    string getPlatnomor() { return plat_nomor; }
    string getMerk() { return merk; }
    int getTahun() { return tahun; }
};

class Pesanan {
private:
    string id_pesanan;
    string id_pengguna;
    string id_driver;
    double jarak_tempuh;
    double biaya;
public:
    Pesanan(string id_pesanan, string id_pengguna, string id_driver, double jarak_tempuh, double biaya) {
        this->id_pesanan = id_pesanan;
        this->id_pengguna = id_pengguna;
        this->id_driver = id_driver;
        this->jarak_tempuh = jarak_tempuh;
        this->biaya = biaya;
    }
    string getIdPesanan() { return id_pesanan; }
    string getIdPengguna() { return id_pengguna; }
    string getIdDriver() { return id_driver; }
    double getJaraktempuh() { return jarak_tempuh; }
    double getBiaya() { return biaya; }
};

class TRKBApp {
private:
    vector<Pengguna> daftarPengguna;
    vector<Driver*> daftarDriver;
    vector<Pesanan> daftarPesanan;
public:
    void registrasiPengguna(string id_pengguna, string nama) {
        daftarPengguna.push_back(Pengguna(id_pengguna, nama));
        cout << "Pengguna Terdaftar: " << id_pengguna << " - " << nama << endl;
    }
    void registrasiDriver(string id_driver, string nama_driver, string tipe_kendaraan, double tarif_per_km, bool premium = false) {
        if (premium)
            daftarDriver.push_back(new PremiumDriver(id_driver, nama_driver, tipe_kendaraan, tarif_per_km));
        else
            daftarDriver.push_back(new Driver(id_driver, nama_driver, tipe_kendaraan, tarif_per_km));
        cout << "Driver Terdaftar: " << id_driver << " - " << nama_driver << (premium ? " (Premium)" : "") << endl;
    }
    int hitungPerjalananPengguna(string id_pengguna) {
        int count = 0;
        for (auto& p : daftarPesanan)
            if (p.getIdPengguna() == id_pengguna)
                count++;
        return count;
    }
    string statusLoyalitas(string id_pengguna) {
        int jumlah = hitungPerjalananPengguna(id_pengguna);
        if (jumlah >= 10)
            return "Gold";
        else if (jumlah >= 5)
            return "Silver";
        else if (jumlah >= 3)
            return "Bronze";
        else
            return "None";
    }
    void buatPesanan(string id_pesanan, string id_pengguna, string id_driver, double jarak_tempuh) {
        for (auto& d : daftarDriver) {
            if (d->getIdDriver() == id_driver) {
                double biaya = d->hitungtarif(jarak_tempuh);
                string status = statusLoyalitas(id_pengguna);
                if (status == "Gold") {
                    biaya *= 0.8;
                    cout << "Pengguna Gold! Mendapat potongan 20%.\n";
                }
                daftarPesanan.push_back(Pesanan(id_pesanan, id_pengguna, id_driver, jarak_tempuh, biaya));
                cout << "Pesanan berhasil dibuat! Biaya: " << biaya << endl;
                return;
            }
        }
        cout << "Driver tidak ditemukan.\n";
    }
    void buatPesanan(string id_pesanan, string id_pengguna, string id_driver, double jarak_tempuh, double diskon) {
        for (auto& d : daftarDriver) {
            if (d->getIdDriver() == id_driver) {
                double biaya = d->hitungtarif(jarak_tempuh) * (1 - diskon);
                daftarPesanan.push_back(Pesanan(id_pesanan, id_pengguna, id_driver, jarak_tempuh, biaya));
                cout << "Pesanan berhasil dibuat (dengan diskon manual)! Biaya: " << biaya << endl;
                return;
            }
        }
        cout << "Driver tidak ditemukan.\n";
    }
    void laporan() {
        cout << "\n=== Laporan Aktivitas Aplikasi ===\n";
        double totalPendapatan = 0;
        for (auto& p : daftarPesanan) {
            cout << "ID Pesanan: " << p.getIdPesanan()
                 << " | Pengguna: " << p.getIdPengguna()
                 << " | Driver: " << p.getIdDriver()
                 << " | Biaya: " << p.getBiaya() << endl;
            totalPendapatan += p.getBiaya();
        }
        cout << "\nTotal Pendapatan Perusahaan: " << totalPendapatan << endl;
        cout << "\n=== Status Loyalitas Pengguna ===\n";
        for (auto& u : daftarPengguna) {
            string status = statusLoyalitas(u.getIdPengguna());
            int jml = hitungPerjalananPengguna(u.getIdPengguna());
            cout << "Pengguna: " << u.getNama()
                 << " | Perjalanan: " << jml
                 << " | Status: " << status << endl;
        }
    }
    void tampilkanPengguna() {
        cout << "\n=== Daftar Pengguna ===\n";
        for (auto& p : daftarPengguna) {
            cout << "ID: " << p.getIdPengguna() << " | Nama: " << p.getNama() << endl;
        }
    }
    void tampilkanDriver() {
        cout << "\n=== Daftar Driver ===\n";
        for (auto& d : daftarDriver) {
            cout << "ID: " << d->getIdDriver()
                 << "| Nama: " << d->getNamaDriver()
                 << "| Tarif per km: " << d->getTarifPerKm() << endl;
        }
    }
};

int main() {
    TRKBApp app;
    int pilihan = 0;
    while (true) {
        cout << "\n=== Menu TRKB App ===\n";
        cout << "1. Registrasi Pengguna\n";
        cout << "2. Registrasi Driver\n";
        cout << "3. Buat Pesanan (tanpa diskon)\n";
        cout << "4. Buat Pesanan (dengan diskon manual)\n";
        cout << "5. Tampilkan Laporan Aktivitas\n";
        cout << "6. Tampilkan Daftar Pengguna & Driver\n";
        cout << "7. Keluar\n";
        cout << "Pilih opsi: ";
        if (!(cin >> pilihan)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Input tidak valid.\n";
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (pilihan == 1) {
            string id, nama;
            cout << "Masukkan ID Pengguna: ";
            getline(cin, id);
            cout << "Masukkan Nama Pengguna: ";
            getline(cin, nama);
            app.registrasiPengguna(id, nama);
        } else if (pilihan == 2) {
            string id, nama, tipe;
            double tarif;
            char prem;
            cout << "Masukkan ID Driver: ";
            getline(cin, id);
            cout << "Masukkan Nama Driver: ";
            getline(cin, nama);
            cout << "Masukkan Tipe Kendaraan: ";
            getline(cin, tipe);
            cout << "Masukkan Tarif per KM: ";
            if (!(cin >> tarif)) {
                cout << "Tarif tidak valid.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cout << "Premium? (y/n): ";
            cin >> prem;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            app.registrasiDriver(id, nama, tipe, tarif, (prem == 'y' || prem == 'Y'));
        } else if (pilihan == 3) {
            string idp, idu, idd;
            double jarak;
            cout << "Masukkan ID Pesanan: ";
            getline(cin, idp);
            cout << "Masukkan ID Pengguna: ";
            getline(cin, idu);
            cout << "Masukkan ID Driver: ";
            getline(cin, idd);
            cout << "Masukkan Jarak Tempuh (km): ";
            if (!(cin >> jarak)) {
                cout << "Input jarak tidak valid.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            app.buatPesanan(idp, idu, idd, jarak);
        } else if (pilihan == 4) {
            string idp, idu, idd;
            double jarak, diskon;
            cout << "Masukkan ID Pesanan: ";
            getline(cin, idp);
            cout << "Masukkan ID Pengguna: ";
            getline(cin, idu);
            cout << "Masukkan ID Driver: ";
            getline(cin, idd);
            cout << "Masukkan Jarak Tempuh (km): ";
            if (!(cin >> jarak)) {
                cout << "Input jarak tidak valid.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cout << "Masukkan Diskon (0.1 untuk 10%): ";
            if (!(cin >> diskon)) {
                cout << "Input diskon tidak valid.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            app.buatPesanan(idp, idu, idd, jarak, diskon);
        } else if (pilihan == 5) {
            app.laporan();
        } else if (pilihan == 6) {
            app.tampilkanPengguna();
            app.tampilkanDriver();
        } else if (pilihan == 7) {
            cout << "Keluar dari program...\n";
            break;
        } else {
            cout << "Opsi tidak dikenal.\n";
        }
    }
    return 0;
}
