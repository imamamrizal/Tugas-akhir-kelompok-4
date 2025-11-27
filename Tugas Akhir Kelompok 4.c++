#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
// #include <mysql.h>
using namespace std;

// Struktur data untuk kendaraan
struct Kendaraan
{
    string noPlat;
    string jenis; // Motor atau Mobil
    time_t waktuMasuk;
    int slotParkir;
    bool statusKeluar;
};

// Class Sistem Parkir
class SistemParkir
{
private:
    vector<Kendaraan> daftarKendaraan;
    int slotMotor;
    int slotMobil;
    int maxSlotMotor;
    int maxSlotMobil;
    int tarifMotor; // per jam
    int tarifMobil; // per jam
    double totalPendapatan;

public:
    // Constructor
    SistemParkir(int maxMotor, int maxMobil, int hargaMotor, int hargaMobil)
    {
        maxSlotMotor = maxMotor;
        maxSlotMobil = maxMobil;
        slotMotor = 0;
        slotMobil = 0;
        tarifMotor = hargaMotor;
        tarifMobil = hargaMobil;
        totalPendapatan = 0;
    }

    // Fungsi untuk menampilkan menu
    void tampilkanMenu()
    {
        cout << "\n+----------------------------------------------------+\n";
        cout << "|        SISTEM PARKIR MINI KELOMPOK 4 - MENU UTAMA  |\n";
        cout << "+----------------------------------------------------+\n";
        cout << "1. Masuk Parkir\n";
        cout << "2. Keluar Parkir\n";
        cout << "3. Lihat Status Parkir\n";
        cout << "4. Lihat Laporan Pendapatan\n";
        cout << "5. Lihat Daftar Kendaraan\n";
        cout << "6. Keluar Program\n";
        cout << "-------------------------------------------\n";
        cout << "Pilih menu (1-6): ";
    }

    // Kendaraan Masuk
    void masukParkir()
    {
        Kendaraan k;
        cout << "\nMasukkan Nomor Plat: ";
        getline(cin >> ws, k.noPlat);
        cout << "Masukkan Jenis Kendaraan (Motor/Mobil): ";
        getline(cin, k.jenis);

        if (k.jenis == "Motor" || k.jenis == "motor")
        {
            if (slotMotor >= maxSlotMotor)
            {
                cout << "Slot Motor Penuh!\n";
                return;
            }
            slotMotor++;
            k.slotParkir = slotMotor;
        }
        else if (k.jenis == "Mobil" || k.jenis == "mobil")
        {
            if (slotMobil >= maxSlotMobil)
            {
                cout << "Slot Mobil Penuh!\n";
                return;
            }
            slotMobil++;
            k.slotParkir = slotMobil;
        }
        else
        {
            cout << "Jenis kendaraan tidak valid!\n";
            return;
        }

        k.waktuMasuk = time(nullptr);
        k.statusKeluar = false;

        daftarKendaraan.push_back(k);

        cout << "Kendaraan berhasil masuk ke slot " << k.slotParkir << ".\n";
    }

    // Kendaraan Keluar
    void keluarParkir()
    {
        string plat;
        cout << "\nMasukkan Nomor Plat: ";
        getline(cin >> ws, plat);

        for (auto &k : daftarKendaraan)
        {
            if (k.noPlat == plat && !k.statusKeluar)
            {
                time_t waktuKeluar = time(nullptr);
                double durasi = difftime(waktuKeluar, k.waktuMasuk) / 3600.0;
                if (durasi < 1)
                    durasi = 1; // Minimal 1 jam

                double biaya = 0;

                if (k.jenis == "Motor" || k.jenis == "motor")
                {
                    biaya = durasi * tarifMotor;
                    slotMotor--;
                }
                else
                {
                    biaya = durasi * tarifMobil;
                    slotMobil--;
                }

                totalPendapatan += biaya;
                k.statusKeluar = true;

                cout << "\n--- Rincian Pembayaran ---\n";
                cout << "Plat Nomor : " << k.noPlat << endl;
                cout << "Jenis      : " << k.jenis << endl;
                cout << "Durasi     : " << fixed << setprecision(1) << durasi << " jam\n";
                cout << "Total Bayar: Rp " << biaya << "\n";
                cout << "----------------------------\n";
                return;
            }
        }
        cout << "Kendaraan tidak ditemukan atau sudah keluar.\n";
    }

    // Status Slot Parkir
    void statusParkir()
    {
        cout << "\n--- Status Slot Parkir ---\n";
        cout << "Slot Motor : " << slotMotor << " / " << maxSlotMotor << endl;
        cout << "Slot Mobil : " << slotMobil << " / " << maxSlotMobil << endl;
    }

    // Laporan Pendapatan
    void laporanPendapatan()
    {
        cout << "\nTotal Pendapatan: Rp " << totalPendapatan << endl;
    }

    // Daftar Kendaraan
    void daftarKendaraanParkir()
    {
        cout << "\n--- Daftar Kendaraan ---\n";
        if (daftarKendaraan.empty())
        {
            cout << "Belum ada kendaraan.\n";
            return;
        }

        for (const auto &k : daftarKendaraan)
        {
            cout << "Plat: " << k.noPlat
                 << " | Jenis: " << k.jenis
                 << " | Slot: " << k.slotParkir
                 << " | Status: " << (k.statusKeluar ? "Keluar" : "Parkir")
                 << endl;
        }
    }
};

// Fungsi Utama
int main()
{
    SistemParkir sp(5, 5, 2000, 5000); // 5 slot motor, 5 slot mobil

    int pilihan;

    while (true)
    {
        sp.tampilkanMenu();
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan)
        {
        case 1:
            sp.masukParkir();
            break;
        case 2:
            sp.keluarParkir();
            break;
        case 3:
            sp.statusParkir();
            break;
        case 4:
            sp.laporanPendapatan();
            break;
        case 5:
            sp.daftarKendaraanParkir();
            break;
        case 6:
            cout << "Keluar program.\n";
            return 0;
        default:
            cout << "Pilihan tidak valid!\n";
        }
    }

    return 0;
}