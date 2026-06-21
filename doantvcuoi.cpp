#include <iostream>
#include <string>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <conio.h>
#include <sstream>
#include <windows.h>
#include <limits>
#include <vector>
#include <iomanip>
using namespace std;

struct Ngay {
    int ngay, thang, nam;
};

struct NodeMuonTra {
    string MASACH;
    Ngay ngayMuon;
    Ngay ngayTra;
    int trangThai; // 0: Đang mượn, 1: Đã trả, 2: Làm mất
    NodeMuonTra* next;
};

struct NodeDocGia {
    int MATHE;
    string ho;
    string ten;
    string phai;
    int trangThai; // 0: Khóa, 1: Hoạt động
    NodeMuonTra* dsMuonTra; 
    NodeDocGia* left;
    NodeDocGia* right;
};

struct NodeSach {
    string MASACH;
    int trangThai; // 0: Mượn được, 1: Đã cho mượn, 2: Đã thanh lý
    string viTri;
    NodeSach* next;
};

struct DauSach {
    string ISBN;
    string tenSach;
    int soTrang;
    string tacGia;
    int namXB;
    string theLoai;
    int soLuotMuon; 
    NodeSach* dms;  
};

const int MAX_DAUSACH = 10000;
const int MAX_LEN_ISBN = 15;
const int MAX_LEN_TEN_SACH = 35;
const int MAX_LEN_TAC_GIA = 30;
const int MAX_LEN_THE_LOAI = 25;
const int MAX_LEN_VI_TRI = 20;
const int MAX_LEN_HO = 25;
const int MAX_LEN_TEN = 15;
const int SO_NGAY_MUON_TOI_DA = 7;
DauSach* dsDauSach[MAX_DAUSACH]; 
int soLuongDauSach = 0;

string boDau(string s) {
    string a = "àáạảãâầấậẩẫăằắặẳẵèéẹẻẽêềếệểễìíịỉĩòóọỏõôồốộổỗơờớợởỡùúụủũưừứựửữỳýỵỷỹđ";
    string b = "aaaaaaaaaaaaaaaaaeeeeeeeeeeeiiiiiooooooooooooooooouuuuuuuuuuuyyyyyd";

    for (int i = 0; i < s.length(); i++) {
        char c = tolower(s[i]);
        for (int j = 0; j < a.length(); j++) {
            if (c == a[j]) {
                s[i] = b[j];
                break;
            }
        }
        s[i] = tolower(s[i]);
    }
    return s;
}
vector<DauSach*> goiYSach(string tuKhoa) {
    vector<DauSach*> kq;

    if (tuKhoa.empty()) return kq;
    
    string key = boDau(tuKhoa);

    for (int i = 0; i < soLuongDauSach; i++) {
        string ten = boDau(dsDauSach[i]->tenSach);

        if (ten.find(key) != string::npos) {
            kq.push_back(dsDauSach[i]);
        }
    }
    return kq;
}
void gotoXY(int x, int y);
void veKhung(string title);
void veKhungCao(string title, int hCanDung);
void drawDoubleBox(int x, int y, int w, int h, string title);
void xoaManHinh();
void anConTro();
void hienConTro();
void xoaVungTrongKhung(int y1, int y2);

extern int khung_X;
extern int khung_Y;
extern int khung_W;
extern int khung_H;
extern int dongCuoiNoiDung;

int toaDoX(int xCu);
int toaDoY(int yCu);
void gotoxyKhung(int xCu, int yCu);
int menuUI_pro(string menu[], int n, string title);
string nhapChuoiTenHopLe(string nhan, int dong);
string nhapPhaiHopLe(int dong);
string catKhoangTrang(string s);
string nhapTenSachRealtime() {
    string input = "";
    char c;

    while (kbhit()) _getch();

    anConTro();
    veKhung("TIM KIEM SACH");

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Nhap ten sach: ";

    gotoXY(toaDoX(28), toaDoY(15));
    cout << "(Enter: Chon goi y dau tien | Backspace: Xoa)";

    while (true) {
        gotoXY(toaDoX(43), toaDoY(4));
        cout << string(35, ' ');
        gotoXY(toaDoX(43), toaDoY(4));
        cout << input;

        for (int i = 0; i < 7; i++) {
            gotoXY(toaDoX(28), toaDoY(6 + i));
            cout << string(khung_W - 8, ' ');
        }

        vector<DauSach*> ds = goiYSach(input);
        if (input.empty()) {
            for (int i = 0; i < soLuongDauSach && i < 7; i++) ds.push_back(dsDauSach[i]);
        }
        for (int i = 0; i < (int)ds.size() && i < 7; i++) {
            string ten = ds[i]->tenSach;
            if (ten.length() > 34) ten = ten.substr(0, 31) + "...";
            gotoXY(toaDoX(28), toaDoY(6 + i));
            cout << " - " << ds[i]->ISBN << " | " << ten;
        }

        gotoXY(toaDoX(43 + input.length()), toaDoY(4));

        c = _getch();

        if (c == 13) {
            hienConTro();
            input = catKhoangTrang(input);
            if (input.empty() && !ds.empty()) return ds[0]->tenSach;
            return input;
        }
        else if (c == 8) {
            if (!input.empty()) input.pop_back();
        }
        else if (isprint(c)) {
            if (input.length() < 30) input += c;
        }
    }
}

// =========================================================


const int MAX_DOCGIA = 10000;
const int MIN_MATHE = 10000;
const int MAX_MATHE = 19999;
int khoMaThe[MAX_DOCGIA]; 
int viTriHienTai = 0;     

// =========================================================

void khoiTaoKhoMaThe() {
    for (int i = 0; i < MAX_DOCGIA; ++i) {
        khoMaThe[i] = MIN_MATHE + i;
    }
    srand(time(0));
    for (int i = MAX_DOCGIA - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(khoMaThe[i], khoMaThe[j]);
    }
    viTriHienTai = 0; 
}

int layMaTheTuDong() {
    if (viTriHienTai >= MAX_DOCGIA) {
        cout << "Loi: Thu vien da dat gioi han so luong doc gia!" << endl;
        return -1; 
    }
    int maMoi = khoMaThe[viTriHienTai];
    viTriHienTai++;
    return maMoi;
}

// =========================================================
NodeDocGia* timDocGia(NodeDocGia* root, int maThe);
	void gotoXY(int x, int y);
	void veKhung(string title);
	void veKhungCao(string title, int hCanDung);
	void inDanhSachTheoMaThe(NodeDocGia* root, int &dong);
	void inDanhSachTheoTenHo(NodeDocGia* root, int &dong);
	void sapXepTheoTenHo(NodeDocGia* arr[], int n);
NodeDocGia* taoNodeDocGia(int maThe, string ho, string ten, string phai) {
    NodeDocGia* p = new NodeDocGia;
    p->MATHE = maThe;
    p->ho = ho;
    p->ten = ten;
    p->phai = phai;
    p->trangThai = 1; 
    p->dsMuonTra = NULL; 
    p->left = NULL;
    p->right = NULL;
    return p;
}

void themDocGiaVaoCay(NodeDocGia* &root, NodeDocGia* p) {
    if (p == NULL) return; 

    if (root == NULL) {
        root = p;
    } else {
        if (p->MATHE < root->MATHE) {
            themDocGiaVaoCay(root->left, p);
        } else if (p->MATHE > root->MATHE) {
            themDocGiaVaoCay(root->right, p);
        } else {
            cout << "Canh bao: Ma the " << p->MATHE << " da ton tai!" << endl;
            delete p; 
        }
    }
}
bool hieuChinhDocGia(NodeDocGia* root, int maThe) {
    NodeDocGia* dg = timDocGia(root, maThe);

    if (dg == NULL) {
        gotoXY(toaDoX(28), toaDoY(14));
        cout << "Khong tim thay doc gia!";
        dongCuoiNoiDung = toaDoY(14);
        return false;
    }

    while (kbhit()) _getch();
    veKhung("HIEU CHINH DOC GIA");

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Ma the : " << dg->MATHE;
    gotoXY(toaDoX(28), toaDoY(5));
    cout << "Ho ten : " << dg->ho << " " << dg->ten;
    gotoXY(toaDoX(28), toaDoY(6));
    cout << "Phai   : " << dg->phai;

    gotoXY(toaDoX(28), toaDoY(8));
    cout << "1. Sua ho";
    gotoXY(toaDoX(28), toaDoY(9));
    cout << "2. Sua ten";
    gotoXY(toaDoX(28), toaDoY(10));
    cout << "3. Sua phai";

    string chon;
    int muc = 0;
    while (true) {
        gotoXY(toaDoX(28), toaDoY(12));
        cout << string(khung_W - 8, ' ');
        gotoXY(toaDoX(28), toaDoY(13));
        cout << string(khung_W - 8, ' ');
        gotoXY(toaDoX(28), toaDoY(12));
        cout << "Chon muc can sua (1/2/3): ";
        getline(cin, chon);
        string chonLoc = "";
        for (int i = 0; i < (int)chon.length(); i++) {
            if (chon[i] != ' ' && chon[i] != '\t' && chon[i] != '\r' && chon[i] != '\n') chonLoc += chon[i];
        }
        chon = chonLoc;

        if (chon == "1" || chon == "2" || chon == "3") {
            muc = stoi(chon);
            break;
        }

        gotoXY(toaDoX(28), toaDoY(13));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "Loi: Chi duoc nhap 1, 2 hoac 3!";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        while (_getch() != 13);
    }

    gotoXY(toaDoX(28), toaDoY(14));
    cout << string(khung_W - 8, ' ');
    gotoXY(toaDoX(28), toaDoY(15));
    cout << string(khung_W - 8, ' ');
    gotoXY(toaDoX(28), toaDoY(16));
    cout << string(khung_W - 8, ' ');

    if (muc == 1) {
        string hoMoi = nhapChuoiTenHopLe("Ho moi", 15);
        dg->ho = hoMoi;
    }
    else if (muc == 2) {
        string tenMoi = nhapChuoiTenHopLe("Ten moi", 15);
        dg->ten = tenMoi;
    }
    else if (muc == 3) {
        string phaiMoi = nhapPhaiHopLe(15);
        dg->phai = phaiMoi;
    }

    gotoXY(toaDoX(28), toaDoY(17));
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << "Cap nhat doc gia thanh cong!";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    dongCuoiNoiDung = toaDoY(17);
    return true;
}
// --- Các hàm hỗ trợ In danh sách Độc giả ---
void inDanhSachTheoMaThe(NodeDocGia* root, int &dong) {

    if (root == NULL)
        return;

    inDanhSachTheoMaThe(root->left, dong);

    gotoXY(toaDoX(28), toaDoY(dong));

    cout << "Ma: " << root->MATHE
         << " | Ho ten: "
         << root->ho << " "
         << root->ten
         << " | Phai: "
         << root->phai;

    dongCuoiNoiDung = toaDoY(dong);
    dong++;

    inDanhSachTheoMaThe(root->right, dong);
}

void doCayRaMangConTro(NodeDocGia* root, NodeDocGia* arr[], int &n) {
    if (root != NULL) {
        arr[n++] = root; 
        doCayRaMangConTro(root->left, arr, n);
        doCayRaMangConTro(root->right, arr, n);
    }
}

void hoanVi(NodeDocGia* &a, NodeDocGia* &b) {
    NodeDocGia* temp = a;
    a = b;
    b = temp;
}

void sapXepTheoTenHo(NodeDocGia* arr[], int n) {

    for (int i = 0; i < n - 1; i++) {

        for (int j = i + 1; j < n; j++) {

            if (arr[i]->ten > arr[j]->ten) {

                NodeDocGia* temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }

            else if (arr[i]->ten == arr[j]->ten) {

                if (arr[i]->ho > arr[j]->ho) {

                    NodeDocGia* temp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = temp;
                }
            }
        }
    }
}
void inDanhSachTheoTenHo(NodeDocGia* root, int &dong) {
    if (root == NULL) {
        gotoXY(toaDoX(28), toaDoY(dong));
        cout << "Danh sach doc gia rong!";
        return;
    }
    
    NodeDocGia* arr[MAX_DOCGIA]; 
    int n = 0;
    
    doCayRaMangConTro(root, arr, n); 
    sapXepTheoTenHo(arr, n);         
    
    for (int i = 0; i < n; i++) {
        gotoXY(toaDoX(28), toaDoY(dong + i));
        cout << "Ma: " << arr[i]->MATHE 
             << " | Ho ten: " << arr[i]->ho << " " << arr[i]->ten 
             << " | Phai: " << arr[i]->phai;
        dongCuoiNoiDung = toaDoY(dong + i);
    }
}
// --- Các hàm hỗ trợ Xóa Độc giả ---
bool kiemTraDangMuonSach(NodeDocGia* p) {
    if (p == NULL) return false;
    return p->dsMuonTra != NULL;
}

void timNodeTheMang(NodeDocGia* &X, NodeDocGia* &Y) {
    if (Y->left != NULL) {
        timNodeTheMang(X, Y->left);
    } else {
        X->MATHE = Y->MATHE;
        X->ho = Y->ho;
        X->ten = Y->ten;
        X->phai = Y->phai;
        X->trangThai = Y->trangThai;
        X->dsMuonTra = Y->dsMuonTra;
        X = Y; 
        Y = Y->right; 
    }
}

bool xoaDocGia(NodeDocGia* &root, int maTheCanXoa) {
    if (root == NULL) return false; 

    if (maTheCanXoa < root->MATHE) {
        return xoaDocGia(root->left, maTheCanXoa);
    } else if (maTheCanXoa > root->MATHE) {
        return xoaDocGia(root->right, maTheCanXoa);
    } else {
        if (kiemTraDangMuonSach(root)) {
            gotoXY(toaDoX(28), toaDoY(15));

			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			
			cout << "Khong duoc xoa doc gia da tung muon sach!";
			
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            return false;
        }

        NodeDocGia* X = root; 
        if (root->left == NULL) {
            root = root->right;
        } else if (root->right == NULL) {
            root = root->left;
        } else {
            timNodeTheMang(X, root->right);
        }
        delete X; 
        return true;
    }
}
NodeSach* timCuonSach(string maSach, int &indexDauSach);
string catKhoangTrang(string s);
string chuanHoaViTriNganKe(string viTri);
bool hopLeViTriNganKe(string viTri);
bool viTriDaTonTai(string viTri);
string chuanHoaChuoiTenNhap(string s);
bool hopLeHoTen(string s);
string nhapChuoiTenHopLe(string nhan, int dong);
string nhapPhaiHopLe(int dong);
string nhapISBNTheoTenRealtime(string title);
string nhapMaSachTheoTenRealtime(string title, bool chiSachRanh, NodeDocGia* dg);
int demSachDangMuon(NodeDocGia* dg);
string ngayThanhChuoi(Ngay n);
void lietKeSachDangMuon(NodeDocGia* root, int maThe) {
    NodeDocGia* dg = timDocGia(root, maThe);

    if (dg == NULL) {
        gotoXY(toaDoX(28), toaDoY(4));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "Khong tim thay doc gia!";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        dongCuoiNoiDung = toaDoY(4);
        return;
    }

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;

    gotoXY(toaDoX(28), toaDoY(5));
    cout << "So sach dang muon: " << demSachDangMuon(dg) << "/3";

    int dongIn = 7;
    bool coSach = false;

    gotoXY(toaDoX(28), toaDoY(dongIn++));
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "MA SACH        TEN SACH                         NGAY MUON";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    gotoXY(toaDoX(28), toaDoY(dongIn++));
    cout << "--------------------------------------------------------------";

    for (NodeMuonTra* k = dg->dsMuonTra; k != NULL; k = k->next) {
        if (k->trangThai == 0) {
            coSach = true;

            int indexDS = -1;
            timCuonSach(k->MASACH, indexDS);
            string tenSach = (indexDS != -1) ? dsDauSach[indexDS]->tenSach : "Khong ro";
            if ((int)tenSach.length() > 28) tenSach = tenSach.substr(0, 25) + "...";

            gotoXY(toaDoX(28), toaDoY(dongIn));
            cout << k->MASACH;
            if ((int)k->MASACH.length() < 14) cout << string(14 - k->MASACH.length(), ' ');
            cout << tenSach;
            if ((int)tenSach.length() < 32) cout << string(32 - tenSach.length(), ' ');
            cout << ngayThanhChuoi(k->ngayMuon);
            dongIn++;
        }
    }

    if (!coSach) {
        gotoXY(toaDoX(28), toaDoY(dongIn++));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        cout << "Doc gia khong muon cuon sach nao.";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
    }

    dongCuoiNoiDung = toaDoY(dongIn);
}
// =========================================================
void luuFileSach();
void luuFileDauSach();
void timSachTheoTheLoai(string theLoai);
vector<string> goiYTheLoai(string tuKhoa) {
    vector<string> kq;

    if (tuKhoa.empty()) return kq;

    string key = boDau(tuKhoa);

    for (int i = 0; i < soLuongDauSach; i++) {
        string tl = boDau(dsDauSach[i]->theLoai);

        if (tl.find(key) != string::npos) {

            bool daCo = false;
            for (string &x : kq) {
                if (x == dsDauSach[i]->theLoai) {
                    daCo = true;
                    break;
                }
            }
            if (!daCo) {
                kq.push_back(dsDauSach[i]->theLoai);
            }
        }
    }

    return kq;
}
string nhapTheLoaiRealtime() {
    string input = "";
    char c;

    while (kbhit()) _getch();

    anConTro();
    veKhung("TIM KIEM THE LOAI");

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Nhap the loai: ";

    gotoXY(toaDoX(28), toaDoY(5));
    cout << "--------------------------";

    gotoXY(toaDoX(28), toaDoY(15));
    cout << "(Enter: Chon | Backspace: Xoa)";

    while (true) {
        gotoXY(toaDoX(43), toaDoY(4));
        cout << string(35, ' ');
        gotoXY(toaDoX(43), toaDoY(4));
        cout << input;

        for (int i = 0; i < 7; i++) {
            gotoXY(toaDoX(28), toaDoY(6 + i));
            cout << string(khung_W - 8, ' ');
        }

        vector<string> ds = goiYTheLoai(input);
        if (!input.empty()) {
            for (int i = 0; i < ds.size() && i < 7; i++) {
                gotoXY(toaDoX(28), toaDoY(6 + i));
                cout << " - " << ds[i];
            }
        }

        gotoXY(toaDoX(43 + input.length()), toaDoY(4));

        c = _getch();

        if (c == 13) {
            hienConTro();
            return input;
        }
        else if (c == 8) {
            if (!input.empty()) input.pop_back();
        }
        else if (isprint(c)) {
            if (input.length() < 30) input += c;
        }
    }
}

void timSachTheoTheLoai(string theLoai) {
    if (soLuongDauSach == 0) {
        xoaManHinh();
        veKhung("KET QUA TIM THE LOAI");
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Kho sach rong!";
        return;
    }

    string key = boDau(theLoai);

    vector<DauSach*> ketQua;

    for (int i = 0; i < soLuongDauSach; i++) {
        string tl = boDau(dsDauSach[i]->theLoai);

        if (tl.find(key) != string::npos) {
            ketQua.push_back(dsDauSach[i]);
        }
    }

    if (ketQua.empty()) {
        xoaManHinh();
        veKhung("KET QUA TIM THE LOAI");

        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Khong tim thay the loai hop le!";
        return;
    }

    int hCanDung = (int)ketQua.size() * 7 + 8;
    if (hCanDung < 18) hCanDung = 18;

    veKhungCao("KET QUA TIM THE LOAI", hCanDung);

    int dong = 4;

    for (int i = 0; i < (int)ketQua.size(); i++) {
        DauSach* ds = ketQua[i];

        gotoXY(toaDoX(28), toaDoY(dong++));
        cout << "ISBN    : " << ds->ISBN;

        gotoXY(toaDoX(28), toaDoY(dong++));
        cout << "Ten     : " << ds->tenSach;

        gotoXY(toaDoX(28), toaDoY(dong++));
        cout << "Tac gia : " << ds->tacGia;

        gotoXY(toaDoX(28), toaDoY(dong++));
        cout << "Nam XB  : " << ds->namXB;

        gotoXY(toaDoX(28), toaDoY(dong++));
        cout << "The loai: " << ds->theLoai;

        gotoXY(toaDoX(28), toaDoY(dong++));
        cout << "--------------------------------------------------";

        dong++;
    }

    dongCuoiNoiDung = toaDoY(dong);
}
void timSachTheoTen(string tuKhoa) {
    xoaManHinh();
    if (soLuongDauSach == 0) {
        veKhung("KET QUA TIM KIEM");
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Kho sach rong!";
        dongCuoiNoiDung = toaDoY(4);
        return;
    }
    string key = boDau(tuKhoa);
    DauSach* dsTimThay = NULL;
    for (int i = 0; i < soLuongDauSach; i++) {
        string ten = boDau(dsDauSach[i]->tenSach);

        if (ten.find(key) != string::npos) {
            dsTimThay = dsDauSach[i];
            break;
        }
    }
    if (dsTimThay == NULL) {
        veKhung("KET QUA TIM KIEM");
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Khong tim thay sach phu hop!";
        dongCuoiNoiDung = toaDoY(4);
        return;
    }
    int soSachCon = 0;
    for (NodeSach* p = dsTimThay->dms; p != NULL; p = p->next) {
        soSachCon++;
    }
    int h_dong = 16 + max(soSachCon, 1);
    if (h_dong < 15)
        h_dong = 15;
    drawDoubleBox(
        khung_X,
        khung_Y,
        khung_W,
        h_dong,
        "KET QUA TIM KIEM"
    );
    int dong = 4;
    gotoXY(toaDoX(28), toaDoY(dong++));
    cout << "ISBN    : " << dsTimThay->ISBN;
    gotoXY(toaDoX(28), toaDoY(dong++));
    cout << "Ten     : " << dsTimThay->tenSach;
    gotoXY(toaDoX(28), toaDoY(dong++));
    cout << "Tac gia : " << dsTimThay->tacGia;
    gotoXY(toaDoX(28), toaDoY(dong++));
    cout << "Nam XB  : " << dsTimThay->namXB;
    gotoXY(toaDoX(28), toaDoY(dong++));
    cout << "The loai: " << dsTimThay->theLoai;
    dong++;
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        14
    );
    gotoXY(toaDoX(28), toaDoY(dong++));
    cout << "Danh sach sach hien co:";
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        7
    );
    if (dsTimThay->dms == NULL) {
        gotoXY(toaDoX(30), toaDoY(dong++));
        cout << "(Chua co cuon sach nao trong kho)";
    }
    else {
        for (NodeSach* p = dsTimThay->dms;
             p != NULL;
             p = p->next) {
            gotoXY(toaDoX(30), toaDoY(dong));
            cout << "- " << p->MASACH
                 << " | VT: "
                 << p->viTri
                 << " | TT: ";
            if (p->trangThai == 0) {
                SetConsoleTextAttribute(
                    GetStdHandle(STD_OUTPUT_HANDLE),
                    10
                );
                cout << "Ranh";
            }
            else if (p->trangThai == 1) {
                SetConsoleTextAttribute(
                    GetStdHandle(STD_OUTPUT_HANDLE),
                    12
                );
                cout << "Da muon";
            }
            else {
                SetConsoleTextAttribute(
                    GetStdHandle(STD_OUTPUT_HANDLE),
                    8
                );
                cout << "Thanh ly";
            }

            SetConsoleTextAttribute(
                GetStdHandle(STD_OUTPUT_HANDLE),
                7
            );
            dong++;
        }
    }
    dongCuoiNoiDung = toaDoY(dong);
}
int timViTriDauSach(string ISBN) {
    for (int i = 0; i < soLuongDauSach; i++) {
        if (dsDauSach[i]->ISBN == ISBN) {
            return i;
        }
    }
    return -1;
}

bool themDauSach(string ISBN, string tenSach, int soTrang, string tacGia, int namXB, string theLoai) {
    if (soLuongDauSach >= MAX_DAUSACH) {
        cout << "Loi: Thu vien da dat gioi han so luong dau sach!" << endl;
        return false;
    }
    if (timViTriDauSach(ISBN) != -1) {
        cout << "Loi: Ma ISBN " << ISBN << " da ton tai!" << endl;
        return false;
    }
    
    DauSach* ds = new DauSach;
    ds->ISBN = ISBN;
    ds->tenSach = tenSach;
    ds->soTrang = soTrang;
    ds->tacGia = tacGia;
    ds->namXB = namXB;
    ds->theLoai = theLoai;
    ds->soLuotMuon = 0; 
    ds->dms = NULL;     
    
    dsDauSach[soLuongDauSach] = ds;
    soLuongDauSach++;
    
    luuFileDauSach();
    luuFileSach();
    return true;
}
void themDauSachTuFile(
    string ISBN,
    string tenSach,
    int soTrang,
    string tacGia,
    int namXB,
    string theLoai)
{
    DauSach* ds = new DauSach;

    ds->ISBN = ISBN;
    ds->tenSach = tenSach;
    ds->soTrang = soTrang;
    ds->tacGia = tacGia;
    ds->namXB = namXB;
    ds->theLoai = theLoai;
    ds->soLuotMuon = 0;
    ds->dms = NULL;

    dsDauSach[soLuongDauSach++] = ds;
}
int demSoLuongSachCon(NodeSach* head) {
    int count = 0;
    for (NodeSach* p = head; p != NULL; p = p->next) count++;
    return count;
}

string layMaSachMoiTheoISBN(string ISBN) {
    int index = timViTriDauSach(ISBN);
    if (index == -1) return "";
    int stt = demSoLuongSachCon(dsDauSach[index]->dms) + 1;
    return ISBN + "-" + to_string(stt);
}

bool themSachVaoDauSach(string ISBN, string viTri) {
    int index = timViTriDauSach(ISBN);
    if (index == -1) {
        cout << "Loi: Khong tim thay ma ISBN " << ISBN << " trong he thong!" << endl;
        return false;
    }

    viTri = chuanHoaViTriNganKe(viTri);
    if (!hopLeViTriNganKe(viTri)) {
        cout << "Loi: Vi tri phai theo dang Ngan-Ke (VD: A1-B2)" << endl;
        return false;
    }
    if (viTriDaTonTai(viTri)) {
        cout << "Loi: Vi tri nay da co sach!" << endl;
        return false;
    }
    
    int stt = demSoLuongSachCon(dsDauSach[index]->dms) + 1;
    string maSachMoi = ISBN + "-" + to_string(stt);
    
    NodeSach* p = new NodeSach;
    p->MASACH = maSachMoi;
    p->trangThai = 0; 
    p->viTri = viTri;
    p->next = NULL;
    
    if (dsDauSach[index]->dms == NULL) {
        dsDauSach[index]->dms = p;
    } else {
        NodeSach* last = dsDauSach[index]->dms;
        while (last->next != NULL) {
            last = last->next;
        }
        last->next = p;
    }
    luuFileDauSach();
    luuFileSach();
    return true;
}

void inDanhSachDauSach() {
    if (soLuongDauSach == 0) {
        veKhung("DANH SACH DAU SACH THEO THE LOAI");
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Kho sach hien tai dang trong!";
        dongCuoiNoiDung = toaDoY(6);
        return;
    }
    DauSach* temp[MAX_DAUSACH];
    for (int i = 0; i < soLuongDauSach; i++)
        temp[i] = dsDauSach[i];
    for (int i = 0; i < soLuongDauSach - 1; i++) {
        for (int j = i + 1; j < soLuongDauSach; j++) {
            string tl1 = boDau(temp[i]->theLoai);
            string tl2 = boDau(temp[j]->theLoai);
            string ten1 = boDau(temp[i]->tenSach);
            string ten2 = boDau(temp[j]->tenSach);
            if (tl1 > tl2 || (tl1 == tl2 && ten1 > ten2))
                swap(temp[i], temp[j]);
        }
    }
    int soTheLoai = 0;
    string tlCu = "";
    for (int i = 0; i < soLuongDauSach; i++) {
        if (i == 0 || boDau(temp[i]->theLoai) != boDau(tlCu)) {
            soTheLoai++;
            tlCu = temp[i]->theLoai;
        }
    }
    int tongDong = 8 + soLuongDauSach + soTheLoai * 5;
    xoaManHinh();
    veKhungCao(
        "DANH SACH DAU SACH THEO THE LOAI",
        tongDong
    );
    int x = khung_X + 3;
    int y = khung_Y + 3;
    int maxTen = khung_W - 55;
    if (maxTen < 18) maxTen = 18;
    if (maxTen > 35) maxTen = 35;
    string theLoaiHienTai = "";
    for (int i = 0; i < soLuongDauSach; i++) {
        DauSach* ds = temp[i];
        if (i == 0 ||
            boDau(ds->theLoai) != boDau(theLoaiHienTai)) {
            theLoaiHienTai = ds->theLoai;
            if (i != 0)
                y++;
            SetConsoleTextAttribute(
                GetStdHandle(STD_OUTPUT_HANDLE),
                14
            );
            gotoXY(x, y++);
            cout << "THE LOAI: "
                 << theLoaiHienTai;
            SetConsoleTextAttribute(
                GetStdHandle(STD_OUTPUT_HANDLE),
                11
            );
            gotoXY(x, y++);
            cout << "ISBN"
                 << string(12, ' ')
                 << "TEN SACH"
                 << string(maxTen - 7, ' ')
                 << "TAC GIA"
                 << string(12, ' ')
                 << "NAM  SL";

            SetConsoleTextAttribute(
                GetStdHandle(STD_OUTPUT_HANDLE),
                8
            );
            gotoXY(x, y++);
            cout << string(khung_W - 8, '-');

            SetConsoleTextAttribute(
                GetStdHandle(STD_OUTPUT_HANDLE),
                7
            );
        }
        string ten = ds->tenSach;
        string tg = ds->tacGia;
        string isbn = ds->ISBN;

        if ((int)ten.length() > maxTen)
            ten = ten.substr(0, maxTen - 3) + "...";
        if ((int)tg.length() > 18)
            tg = tg.substr(0, 15) + "...";
        int soCuon =
            demSoLuongSachCon(ds->dms);
        gotoXY(x, y);
        cout << isbn;
        gotoXY(x + 16, y);
        cout << ten;
        gotoXY(x + 18 + maxTen, y);
        cout << tg;
        gotoXY(x + 40 + maxTen, y);
        cout << ds->namXB
             << "   "
             << soCuon;

        y++;
    }
    dongCuoiNoiDung = y;
}

void inTop10SachMuonNhieu() {
    if (soLuongDauSach == 0) {
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Kho sach rong!";
        return;
    }

    DauSach* temp[MAX_DAUSACH];
    for (int i = 0; i < soLuongDauSach; i++) {
        temp[i] = dsDauSach[i];
    }

    for (int i = 0; i < soLuongDauSach - 1; i++) {
        for (int j = i + 1; j < soLuongDauSach; j++) {
            if (temp[i]->soLuotMuon < temp[j]->soLuotMuon) {
                swap(temp[i], temp[j]);
            }
        }
    }

    int limit = min(10, soLuongDauSach);
    int dongIn = 4; 

    for (int i = 0; i < limit; i++) {
        gotoXY(toaDoX(28), toaDoY(dongIn));
        string tenRutGon = temp[i]->tenSach;
        if (tenRutGon.length() > 25) {
            tenRutGon = tenRutGon.substr(0, 22) + "...";
        }
        
        cout << i + 1 << ". " << tenRutGon 
             << " | Luot muon: " << temp[i]->soLuotMuon;
        dongIn++;
    }
}
// =========================================================

NodeDocGia* timDocGia(NodeDocGia* root, int maThe) {
    if (root == NULL) return NULL;
    if (root->MATHE == maThe) return root;
    if (maThe < root->MATHE) return timDocGia(root->left, maThe);
    return timDocGia(root->right, maThe);
}

string layISBNTuMaSach(string maSach) {
    size_t pos = maSach.find('-');
    if (pos != string::npos) {
        return maSach.substr(0, pos);
    }
    return "";
}

NodeSach* timCuonSach(string maSach, int &indexDauSach) {
    string ISBN = layISBNTuMaSach(maSach);
    indexDauSach = timViTriDauSach(ISBN);
    
    if (indexDauSach == -1) return NULL;
    
    for (NodeSach* p = dsDauSach[indexDauSach]->dms; p != NULL; p = p->next) {
        if (p->MASACH == maSach) return p;
    }
    return NULL;
}

Ngay layNgayHeThong() {
    time_t bayGio = time(0);
    tm* ltm = localtime(&bayGio);
    Ngay n;
    n.ngay = ltm->tm_mday;
    n.thang = ltm->tm_mon + 1;
    n.nam = ltm->tm_year + 1900;
    return n;
}

string ngayThanhChuoi(Ngay n) {
    stringstream ss;
    if (n.ngay < 10) ss << "0";
    ss << n.ngay << "/";
    if (n.thang < 10) ss << "0";
    ss << n.thang << "/" << n.nam;
    return ss.str();
}

time_t doiNgaySangTimeT(Ngay n) {
    tm t = {};
    t.tm_mday = n.ngay;
    t.tm_mon = n.thang - 1;
    t.tm_year = n.nam - 1900;
    t.tm_hour = 12;
    return mktime(&t);
}

int soNgayGiuaHaiNgay(Ngay a, Ngay b) {
    time_t ta = doiNgaySangTimeT(a);
    time_t tb = doiNgaySangTimeT(b);
    double giay = difftime(tb, ta);
    return (int)(giay / 86400);
}

struct ThongTinQuaHan {
    NodeDocGia* docGia;
    string maSach;
    string tenSach;
    Ngay ngayMuon;
    int soNgayQuaHan;
};

void thuThapQuaHan(NodeDocGia* root, vector<ThongTinQuaHan> &ds, Ngay homNay) {
    if (root == NULL) return;

    thuThapQuaHan(root->left, ds, homNay);

    for (NodeMuonTra* mt = root->dsMuonTra; mt != NULL; mt = mt->next) {
        if (mt->trangThai == 0) {
            int soNgayDaMuon = soNgayGiuaHaiNgay(mt->ngayMuon, homNay);
            int quaHan = soNgayDaMuon - SO_NGAY_MUON_TOI_DA;

            if (quaHan > 0) {
                int indexDS = -1;
                timCuonSach(mt->MASACH, indexDS);

                ThongTinQuaHan info;
                info.docGia = root;
                info.maSach = mt->MASACH;
                info.tenSach = (indexDS != -1) ? dsDauSach[indexDS]->tenSach : "Khong ro";
                info.ngayMuon = mt->ngayMuon;
                info.soNgayQuaHan = quaHan;

                ds.push_back(info);
            }
        }
    }

    thuThapQuaHan(root->right, ds, homNay);
}

void inDanhSachDocGiaQuaHan(NodeDocGia* root) {
    vector<ThongTinQuaHan> dsQuaHan;
    Ngay homNay = layNgayHeThong();

    thuThapQuaHan(root, dsQuaHan, homNay);

    sort(dsQuaHan.begin(), dsQuaHan.end(), [](const ThongTinQuaHan &a, const ThongTinQuaHan &b) {
        if (a.soNgayQuaHan != b.soNgayQuaHan) return a.soNgayQuaHan > b.soNgayQuaHan;
        return a.docGia->MATHE < b.docGia->MATHE;
    });

    int hCanDung = 10 + (int)dsQuaHan.size();
    if (hCanDung < 18) hCanDung = 18;
    veKhungCao("DOC GIA MUON SACH QUA HAN", hCanDung);

    gotoXY(khung_X + 3, khung_Y + 3);
    cout << "Ngay he thong: " << ngayThanhChuoi(homNay)
         << " | Han muon: " << SO_NGAY_MUON_TOI_DA << " ngay";

    if (dsQuaHan.empty()) {
        gotoXY(khung_X + 3, khung_Y + 5);
        cout << "Khong co doc gia muon sach qua han!";
        dongCuoiNoiDung = khung_Y + 6;
        return;
    }

    int y = khung_Y + 5;
    gotoXY(khung_X + 3, y++);
    cout << "STT | Ma the | Ho ten                 | Ma sach      | Ngay muon  | Qua han";
    gotoXY(khung_X + 3, y++);
    cout << "----------------------------------------------------------------------------";

    for (int i = 0; i < (int)dsQuaHan.size(); i++) {
        string hoTen = dsQuaHan[i].docGia->ho + " " + dsQuaHan[i].docGia->ten;
        if (hoTen.length() > 22) hoTen = hoTen.substr(0, 19) + "...";

        gotoXY(khung_X + 3, y++);
        cout << i + 1 << "   | "
             << dsQuaHan[i].docGia->MATHE << "  | "
             << hoTen;

        if (hoTen.length() < 22) cout << string(22 - hoTen.length(), ' ');

        cout << " | " << dsQuaHan[i].maSach;
        if (dsQuaHan[i].maSach.length() < 11) cout << string(11 - dsQuaHan[i].maSach.length(), ' ');

        cout << " | " << ngayThanhChuoi(dsQuaHan[i].ngayMuon)
             << " | " << dsQuaHan[i].soNgayQuaHan << " ngay";
    }

    dongCuoiNoiDung = y;
}

int demSachDangMuon(NodeDocGia* dg) {
    int dem = 0;
    if (dg == NULL) return 0;
    for (NodeMuonTra* k = dg->dsMuonTra; k != NULL; k = k->next) {
        if (k->trangThai == 0) dem++;
    }
    return dem;
}

bool docGiaDangLamMatSach(NodeDocGia* dg) {
    if (dg == NULL) return false;
    for (NodeMuonTra* k = dg->dsMuonTra; k != NULL; k = k->next) {
        if (k->trangThai == 2) return true;
    }
    return false;
}

NodeMuonTra* timPhieuDangMuon(NodeDocGia* dg, string maSach) {
    if (dg == NULL) return NULL;
    for (NodeMuonTra* k = dg->dsMuonTra; k != NULL; k = k->next) {
        if (k->MASACH == maSach && k->trangThai == 0) return k;
    }
    return NULL;
}

string catTheoKhung(string msg) {
    int maxLen = khung_W - 10;
    if (maxLen < 25) maxLen = 25;
    if ((int)msg.length() > maxLen) {
        msg = msg.substr(0, maxLen - 3) + "...";
    }
    return msg;
}

void inLoiChucNang(string msg) {
    msg = catTheoKhung("Loi: " + msg);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    cout << msg;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void inThanhCongChucNang(string msg) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
    cout << msg;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

bool muonSach(NodeDocGia* root, int maThe, string maSach) {
    maSach = catKhoangTrang(maSach);

    NodeDocGia* dg = timDocGia(root, maThe);
    if (dg == NULL) {
        inLoiChucNang("Khong tim thay doc gia!");
        return false;
    }

    if (dg->trangThai != 1) {
        inLoiChucNang("The doc gia dang bi khoa, khong duoc muon sach!");
        return false;
    }

    if (maSach.empty()) {
        inLoiChucNang("Ma sach khong duoc de trong!");
        return false;
    }

    if (maSach.find('-') == string::npos) {
        inLoiChucNang("Ma sach sai dinh dang. Vi du dung: ISBN-1");
        return false;
    }

    if (docGiaDangLamMatSach(dg)) {
        inLoiChucNang("Doc gia dang lam mat sach, khong duoc muon them!");
        return false;
    }

    if (timPhieuDangMuon(dg, maSach) != NULL) {
        inLoiChucNang("Doc gia nay dang muon chinh cuon sach nay!");
        return false;
    }

    int soSachDangMuon = demSachDangMuon(dg);
    if (soSachDangMuon >= 3) {
        inLoiChucNang("Moi doc gia chi duoc muon toi da 3 cuon!");
        return false;
    }

    int indexDS = -1;
    NodeSach* sach = timCuonSach(maSach, indexDS);
    if (sach == NULL || indexDS == -1) {
        inLoiChucNang("Khong tim thay ma sach nay trong he thong!");
        return false;
    }

    if (sach->trangThai == 1) {
        inLoiChucNang("Sach nay da co nguoi muon!");
        return false;
    }

    if (sach->trangThai == 2) {
        inLoiChucNang("Sach nay da thanh ly, khong duoc muon!");
        return false;
    }

    if (sach->trangThai != 0) {
        inLoiChucNang("Trang thai sach khong hop le!");
        return false;
    }

    NodeMuonTra* mtMoi = new NodeMuonTra;
    mtMoi->MASACH = maSach;
    mtMoi->ngayMuon = layNgayHeThong();
    mtMoi->ngayTra = {0, 0, 0};
    mtMoi->trangThai = 0;
    mtMoi->next = dg->dsMuonTra;
    dg->dsMuonTra = mtMoi;

    sach->trangThai = 1;
    dsDauSach[indexDS]->soLuotMuon++;

    inThanhCongChucNang("=> Muon sach thanh cong!");
    cout << " | Ngay muon: " << ngayThanhChuoi(mtMoi->ngayMuon)
         << " | Dang muon: " << demSachDangMuon(dg) << "/3";
    luuFileSach();
    luuFileDauSach();
    return true;
}

bool traSach(NodeDocGia* root, int maThe, string maSach) {
    maSach = catKhoangTrang(maSach);

    NodeDocGia* dg = timDocGia(root, maThe);
    if (dg == NULL) {
        inLoiChucNang("Khong tim thay doc gia!");
        return false;
    }

    if (maSach.empty()) {
        inLoiChucNang("Ma sach khong duoc de trong!");
        return false;
    }

    if (maSach.find('-') == string::npos) {
        inLoiChucNang("Ma sach sai dinh dang. Vi du dung: ISBN-1");
        return false;
    }

    NodeMuonTra* mt = timPhieuDangMuon(dg, maSach);
    if (mt == NULL) {
        inLoiChucNang("Doc gia khong dang muon cuon sach nay!");
        return false;
    }

    int indexDS = -1;
    NodeSach* sach = timCuonSach(maSach, indexDS);
    if (sach == NULL || indexDS == -1) {
        inLoiChucNang("Khong tim thay ma sach nay trong kho, khong the cap nhat trang thai sach!");
        return false;
    }

    if (sach->trangThai == 2) {
        inLoiChucNang("Sach da thanh ly, khong the tra theo trang thai binh thuong!");
        return false;
    }

    mt->trangThai = 1;
    mt->ngayTra = layNgayHeThong();
    sach->trangThai = 0;

    int soNgayDaMuon = soNgayGiuaHaiNgay(mt->ngayMuon, mt->ngayTra);
    int soNgayQuaHan = soNgayDaMuon - SO_NGAY_MUON_TOI_DA;

    inThanhCongChucNang("=> Tra sach thanh cong!");
    cout << " | Ngay tra: " << ngayThanhChuoi(mt->ngayTra);
    cout << " | So ngay da muon: " << soNgayDaMuon;
    if (soNgayQuaHan > 0) cout << " | Qua han: " << soNgayQuaHan << " ngay";
	luuFileSach();
	luuFileDauSach();
    return true;
}
// =========================================================

void ghiNodeRaFile_NLR(NodeDocGia* root, ofstream &outFile) {
    if (root != NULL) {
        outFile << root->MATHE << "," << root->ho << "," << root->ten << "," 
                << root->phai << "," << root->trangThai << endl;
        ghiNodeRaFile_NLR(root->left, outFile);
        ghiNodeRaFile_NLR(root->right, outFile);
    }
}

string catKhoangTrang(string s) {
    size_t dau = s.find_first_not_of(" \t\r\n");
    if (dau == string::npos) return "";
    size_t cuoi = s.find_last_not_of(" \t\r\n");
    return s.substr(dau, cuoi - dau + 1);
}
bool laSoNguyen(string s) {
    s = catKhoangTrang(s);
    if (s.empty()) return false;
    for (int i = 0; i < (int)s.length(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

string chuanHoaChuoiTenNhap(string s) {
    string kq = "";
    bool daCoChu = false, vuaCoKhoangTrang = false;
    for (int i = 0; i < (int)s.length(); i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            if (daCoChu && !vuaCoKhoangTrang) {
                kq += ' ';
                vuaCoKhoangTrang = true;
            }
        } else {
            kq += s[i];
            daCoChu = true;
            vuaCoKhoangTrang = false;
        }
    }
    while (!kq.empty() && kq[kq.length() - 1] == ' ') kq.pop_back();
    return kq;
}

bool hopLeHoTen(string s) {
    s = chuanHoaChuoiTenNhap(s);
    if (s.empty()) return false;
    for (int i = 0; i < (int)s.length(); i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == ' ') continue;
        if (c >= 'A' && c <= 'Z') continue;
        if (c >= 'a' && c <= 'z') continue;
        if (c >= 128) continue;
        return false;
    }
    return true;
}

void xoaDongNhap(int dong) {
    gotoXY(toaDoX(28), toaDoY(dong));
    cout << string(khung_W - 8, ' ');
}

string nhapChuoiTenHopLe(string nhan, int dong) {
    string s;
    int gioiHan = (boDau(nhan).find("ten") != string::npos) ? MAX_LEN_TEN : MAX_LEN_HO;
    while (true) {
        xoaDongNhap(dong);
        gotoXY(toaDoX(28), toaDoY(dong));
        cout << nhan << ": ";
        s.clear();
		char c;
		
		while (true) {
		    c = _getch();

		    if (c == 13) { // Enter
		        break;
		    }
		
		    if (c == 8) { // Backspace
		        if (!s.empty()) {
		            s.pop_back();
		            cout << "\b \b";
		        }
		        continue;
		    }
		
		    if (c == ' ') {
		        if (s.empty()) {
		            gotoXY(toaDoX(28), toaDoY(dong + 2));
		            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		            cout << "Khong duoc de khoang trang o dau!";
		            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		            gotoXY(
					    toaDoX(28) + (int)nhan.length() + 2 + (int)s.length(),
					    toaDoY(dong)
					);
		            continue;
		        }
		
		        if (s.back() == ' ') {
		            gotoXY(toaDoX(28), toaDoY(dong + 2));
		            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		            cout << "Chi duoc 1 khoang trang giua cac tu!";
		            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		            gotoXY(
					    toaDoX(28) + (int)nhan.length() + 2 + (int)s.length(),
					    toaDoY(dong)
					);
		            continue;
		        }
		    }

		    gotoXY(toaDoX(28), toaDoY(dong + 2));
		    cout << string(khung_W - 8, ' ');
		    gotoXY(toaDoX(28) + (int)nhan.length() + 2 + (int)s.length(), toaDoY(dong));
		    if ((int)s.length() < gioiHan) {
		        s += c;
		        cout << c;
		    }
		}
		
		s = chuanHoaChuoiTenNhap(s);
        if (hopLeHoTen(s) && (int)s.length() <= gioiHan) return s;
        gotoXY(toaDoX(28), toaDoY(dong + 2));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        if ((int)s.length() > gioiHan) cout << "Loi: " << nhan << " toi da " << gioiHan << " ky tu!";
        else cout << "Loi: " << nhan << " chi duoc nhap chu, khong duoc de trong!";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        _getch();
        gotoXY(toaDoX(28), toaDoY(dong + 2)); cout << string(khung_W - 8, ' ');
    }
}

string nhapPhaiHopLe(int dong) {
    string phai;
    while (true) {
        xoaDongNhap(dong);
        gotoXY(toaDoX(28), toaDoY(dong));
        cout << "Phai (Nam/Nu): ";
        getline(cin, phai);
        phai = chuanHoaChuoiTenNhap(phai);
        string key = boDau(phai);
        if (key == "nam") return "Nam";
        if (key == "nu") return "Nu";
        gotoXY(toaDoX(28), toaDoY(dong + 2));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "Loi: Phai chi nhan Nam hoac Nu!";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        _getch();
        gotoXY(toaDoX(28), toaDoY(dong + 2)); cout << string(khung_W - 8, ' ');
    }
}


bool coChuCai(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        unsigned char c = (unsigned char)s[i];
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c >= 128) return true;
    }
    return false;
}

bool hopLeISBN(string s) {
    s = catKhoangTrang(s);
    if (s.empty()) return false;
    if ((int)s.length() > MAX_LEN_ISBN) return false;
    if (timViTriDauSach(s) != -1) return false;
    for (int i = 0; i < (int)s.length(); i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') return false;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_') continue;
        return false;
    }
    return true;
}


string chuanHoaViTriNganKe(string viTri) {
    viTri = catKhoangTrang(viTri);
    string kq = "";
    for (int i = 0; i < (int)viTri.length(); i++) {
        unsigned char c = (unsigned char)viTri[i];
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') continue;
        if (c >= 'a' && c <= 'z') c = c - 32;
        kq += (char)c;
    }
    return kq;
}

bool hopLeMotPhanViTri(string s) {
    s = chuanHoaViTriNganKe(s);
    if ((int)s.length() < 2) return false;
    if (!(s[0] >= 'A' && s[0] <= 'Z')) return false;
    for (int i = 1; i < (int)s.length(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

bool hopLeViTriNganKe(string viTri) {
    viTri = chuanHoaViTriNganKe(viTri);
    if (viTri.empty()) return false;
    if ((int)viTri.length() > MAX_LEN_VI_TRI) return false;

    size_t dauGach = viTri.find('-');
    if (dauGach == string::npos) {
        return hopLeMotPhanViTri(viTri);
    }

    if (viTri.find('-', dauGach + 1) != string::npos) return false;

    string ngan = viTri.substr(0, dauGach);
    string ke = viTri.substr(dauGach + 1);

    return hopLeMotPhanViTri(ngan) && hopLeMotPhanViTri(ke);
}

bool viTriDaTonTai(string viTri) {
    viTri = chuanHoaViTriNganKe(viTri);
    for (int i = 0; i < soLuongDauSach; i++) {
        for (NodeSach* p = dsDauSach[i]->dms; p != NULL; p = p->next) {
            if (chuanHoaViTriNganKe(p->viTri) == viTri) return true;
        }
    }
    return false;
}

bool hopLeChuoiSach(string s) {
    s = chuanHoaChuoiTenNhap(s);
    if (s.empty()) return false;
    if (!coChuCai(s)) return false;
    for (int i = 0; i < (int)s.length(); i++) {
        unsigned char c = (unsigned char)s[i];
        if (c == ' ') continue;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) continue;
        if (c >= '0' && c <= '9') continue;
        if (c >= 128) continue;
        return false;
    }
    return true;
}

void baoLoiNhapDauSach(string msg) {
    int x = toaDoX(28);
    int y = toaDoY(12);
    int maxLen = khung_W - 10;
    if (maxLen < 30) maxLen = 30;

    for (int i = 0; i < 3; i++) {
        gotoXY(x, y + i);
        cout << string(khung_W - 8, ' ');
    }

    msg = "Loi: " + msg;
    string dong1 = msg;
    string dong2 = "";

    if ((int)dong1.length() > maxLen) {
        int cut = maxLen;
        while (cut > 20 && dong1[cut] != ' ') cut--;
        if (cut <= 20) cut = maxLen;
        dong2 = dong1.substr(cut);
        dong1 = dong1.substr(0, cut);
        dong2 = catKhoangTrang(dong2);
        if ((int)dong2.length() > maxLen) dong2 = dong2.substr(0, maxLen - 3) + "...";
    }

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
    gotoXY(x, y);
    cout << dong1;
    if (!dong2.empty()) {
        gotoXY(x, y + 1);
        cout << dong2;
    }
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    gotoXY(x, y + 2);
    cout << "Nhan phim bat ky de nhap lai...";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    _getch();

    for (int i = 0; i < 3; i++) {
        gotoXY(x, y + i);
        cout << string(khung_W - 8, ' ');
    }
}

string nhapISBNHopLe(int dong) {
    string s;
    while (true) {
        xoaDongNhap(dong);
        gotoXY(toaDoX(28), toaDoY(dong)); cout << "ISBN    : ";
        getline(cin, s);
        for (int i = 0; i < (int)s.length(); i++) {
            s[i] = toupper((unsigned char)s[i]);
        }
        if (hopLeISBN(s)) return s;
        if (s.empty()) baoLoiNhapDauSach("ISBN khong duoc de trong");
        else if ((int)s.length() > MAX_LEN_ISBN) baoLoiNhapDauSach("ISBN toi da 15 ky tu");
        else if (timViTriDauSach(s) != -1) baoLoiNhapDauSach("ISBN da ton tai");
        else baoLoiNhapDauSach("ISBN chi nhan chu, so, dau '-' hoac '_'");
    }
}

string nhapChuoiSachHopLe(string nhan, int dong) {
    string s;
    string tenTruong = catKhoangTrang(nhan);
    int gioiHan = MAX_LEN_TEN_SACH;
    if (tenTruong == "Tac gia") gioiHan = MAX_LEN_TAC_GIA;
    if (tenTruong == "The loai") gioiHan = MAX_LEN_THE_LOAI;
    while (true) {
        xoaDongNhap(dong);
        gotoXY(toaDoX(28), toaDoY(dong)); cout << nhan << ": ";
        s.clear();
		char c;
		
		while (true) {
		    c = _getch();
		
		    if (c == 13) break; // Enter
		
		    if (c == 8) { // Backspace
		        if (!s.empty()) {
		            s.pop_back();
		            cout << "\b \b";
		        }
		        continue;
		    }
		
		    if (c == ' ') {
		
		        if (s.empty()) {
		            baoLoiNhapDauSach("Khong duoc de khoang trang o dau");
		
		            gotoXY(
		                toaDoX(28) + (int)nhan.length() + 2 + (int)s.length(),
		                toaDoY(dong)
		            );
		            continue;
		        }
		
		        if (s.back() == ' ') {
		            baoLoiNhapDauSach("Chi duoc 1 khoang trang giua cac tu");
		
		            gotoXY(
		                toaDoX(28) + (int)nhan.length() + 2 + (int)s.length(),
		                toaDoY(dong)
		            );
		            continue;
		        }
		    }

		    gotoXY(toaDoX(28), toaDoY(dong + 2));
		    cout << string(khung_W - 8, ' ');
		
		    gotoXY(
		        toaDoX(28) + (int)nhan.length() + 2 + (int)s.length(),
		        toaDoY(dong)
		    );
		
		    if ((int)s.length() < gioiHan) {
		        s += c;
		        cout << c;
		    }
		}
		
		s = chuanHoaChuoiTenNhap(s);

        if (hopLeChuoiSach(s) && (int)s.length() <= gioiHan) return s;

        if (s.empty()) {
            baoLoiNhapDauSach(tenTruong + " khong duoc de trong");
        }
        else if ((int)s.length() > gioiHan) {
            baoLoiNhapDauSach(tenTruong + " toi da " + to_string(gioiHan) + " ky tu");
        }
        else {
            baoLoiNhapDauSach(tenTruong + " chi nhan chu cai");
        }
    }
}

int nhapSoNguyenDuongHopLe(string nhan, int dong, int minVal, int maxVal) {
    string s;
    long long x;
    string tenTruong = catKhoangTrang(nhan);

    while (true) {
        xoaDongNhap(dong);
        gotoXY(toaDoX(28), toaDoY(dong)); cout << nhan << ": ";
        getline(cin, s);
        s = catKhoangTrang(s);

        if (s.empty()) {
            if (tenTruong == "So trang") baoLoiNhapDauSach("So trang khong duoc de trong");
            else baoLoiNhapDauSach("Nam XB khong duoc de trong");
            continue;
        }

        if (!laSoNguyen(s)) {
            if (tenTruong == "So trang") baoLoiNhapDauSach("So trang phai duoc nhap bang so");
            else baoLoiNhapDauSach("Nam XB phai duoc nhap bang so");
            continue;
        }

        if (s.length() > 9) {
            if (tenTruong == "So trang") baoLoiNhapDauSach("So trang qua lon");
            else baoLoiNhapDauSach("Nam XB qua lon");
            continue;
        }

        x = stoll(s);

        if (x < minVal) {
            if (tenTruong == "So trang") baoLoiNhapDauSach("So trang phai lon hon 0");
            else baoLoiNhapDauSach("Nam XB khong hop le");
            continue;
        }

        if (x > maxVal) {
            if (tenTruong == "So trang") baoLoiNhapDauSach("So trang qua lon");
            else baoLoiNhapDauSach("Nam XB khong duoc lon hon nam hien tai");
            continue;
        }

        return (int)x;
    }
}

vector<DauSach*> goiYISBNTheoTenHoacISBN(string tuKhoa) {
    vector<DauSach*> kq;
    tuKhoa = catKhoangTrang(tuKhoa);
    string key = boDau(tuKhoa);
    for (int i = 0; i < soLuongDauSach; i++) {
        string ten = boDau(dsDauSach[i]->tenSach);
        string isbn = boDau(dsDauSach[i]->ISBN);
        if (key.empty() || ten.find(key) != string::npos || isbn.find(key) != string::npos) kq.push_back(dsDauSach[i]);
    }
    return kq;
}

string nhapISBNTheoTenRealtime(string title) {
    string input = "";
    char c;
    while (kbhit()) _getch();
    anConTro();
    veKhung(title);
    gotoXY(toaDoX(28), toaDoY(4)); cout << "Nhap ten sach/ISBN: ";
    gotoXY(toaDoX(28), toaDoY(15)); cout << "(Enter: Chon goi y dau tien | Backspace: Xoa)";
    while (true) {
        gotoXY(toaDoX(48), toaDoY(4)); cout << string(35, ' ');
        gotoXY(toaDoX(48), toaDoY(4)); cout << input;
        for (int i = 0; i < 7; i++) { gotoXY(toaDoX(28), toaDoY(6 + i)); cout << string(khung_W - 8, ' '); }
        vector<DauSach*> ds = goiYISBNTheoTenHoacISBN(input);
        {
            for (int i = 0; i < (int)ds.size() && i < 7; i++) {
                string ten = ds[i]->tenSach;	
                if (ten.length() > 28) ten = ten.substr(0, 25) + "...";
                gotoXY(toaDoX(28), toaDoY(6 + i)); cout << "- " << ds[i]->ISBN << " | " << ten;
            }
        }
        gotoXY(toaDoX(48 + input.length()), toaDoY(4));
        c = _getch();
        if (c == 13) {
            hienConTro(); input = catKhoangTrang(input);
            for (int i = 0; i < soLuongDauSach; i++) if (boDau(dsDauSach[i]->ISBN) == boDau(input)) return dsDauSach[i]->ISBN;
            ds = goiYISBNTheoTenHoacISBN(input);
            if (!ds.empty()) return ds[0]->ISBN;
            return input;
        } else if (c == 8) { if (!input.empty()) input.pop_back(); }
        else if (isprint((unsigned char)c)) { if (input.length() < 35) input += c; }
    }
}

vector<string> goiYMaSachTheoTen(string tuKhoa, bool chiSachRanh, NodeDocGia* dg) {
    vector<string> kq;
    tuKhoa = catKhoangTrang(tuKhoa);
    string key = boDau(tuKhoa);
    if (dg != NULL) {
        for (NodeMuonTra* mt = dg->dsMuonTra; mt != NULL; mt = mt->next) {
            if (mt->trangThai == 0) {
                int indexDS = -1;
                timCuonSach(mt->MASACH, indexDS);
                string ten = (indexDS != -1) ? boDau(dsDauSach[indexDS]->tenSach) : "";
                string ma = boDau(mt->MASACH);
                if (key.empty() || ten.find(key) != string::npos || ma.find(key) != string::npos) kq.push_back(mt->MASACH);
            }
        }
        return kq;
    }
    for (int i = 0; i < soLuongDauSach; i++) {
        string ten = boDau(dsDauSach[i]->tenSach);
        for (NodeSach* p = dsDauSach[i]->dms; p != NULL; p = p->next) {
            string ma = boDau(p->MASACH);
            if (key.empty() || ten.find(key) != string::npos || ma.find(key) != string::npos) {
                if (!chiSachRanh || p->trangThai == 0) kq.push_back(p->MASACH);
            }
        }
    }
    return kq;
}

string nhapMaSachTheoTenRealtime(string title, bool chiSachRanh, NodeDocGia* dg) {
    string input = "";
    char c;
    while (kbhit()) _getch();
    anConTro();
    veKhung(title);
    gotoXY(toaDoX(28), toaDoY(4)); cout << "Nhap ten sach/ma sach: ";
    gotoXY(toaDoX(28), toaDoY(15)); cout << "(Enter: Chon goi y dau tien | Backspace: Xoa)";
    while (true) {
        gotoXY(toaDoX(51), toaDoY(4)); cout << string(35, ' ');
        gotoXY(toaDoX(51), toaDoY(4)); cout << input;
        for (int i = 0; i < 7; i++) { gotoXY(toaDoX(28), toaDoY(6 + i)); cout << string(khung_W - 8, ' '); }
        vector<string> ds = goiYMaSachTheoTen(input, chiSachRanh, dg);
        {
            for (int i = 0; i < (int)ds.size() && i < 7; i++) {
                int indexDS = -1;
                NodeSach* sach = timCuonSach(ds[i], indexDS);
                string ten = (indexDS != -1) ? dsDauSach[indexDS]->tenSach : "Khong ro";
                if (ten.length() > 24) ten = ten.substr(0, 21) + "...";
                gotoXY(toaDoX(28), toaDoY(6 + i)); cout << "- " << ds[i] << " | " << ten;
                if (sach != NULL) cout << " | VT: " << sach->viTri;
            }
        }
        gotoXY(toaDoX(51 + input.length()), toaDoY(4));
        c = _getch();
        if (c == 13) {
            hienConTro(); input = catKhoangTrang(input);
            int tmp = -1;
            if (timCuonSach(input, tmp) != NULL) return input;
            ds = goiYMaSachTheoTen(input, chiSachRanh, dg);
            if (!ds.empty()) return ds[0];
            return input;
        } else if (c == 8) { if (!input.empty()) input.pop_back(); }
        else if (isprint((unsigned char)c)) { if (input.length() < 35) input += c; }
    }
}

int chuanHoaTrangThaiThe(string s) {
    s = catKhoangTrang(s);
    if (s == "0") return 0;
    return 1;
}

void docDanhSachDocGia(NodeDocGia* &root) {
    ifstream inFile("docgia.txt");

    if (!inFile.is_open()) {
        cout << "Khong tim thay file docgia.txt!\n";
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        vector<string> cot;
        string temp;
        stringstream ss(line);
        while (getline(ss, temp, ',')) {
            cot.push_back(catKhoangTrang(temp));
        }

        if (cot.size() < 4) continue;

        int maThe = layMaTheTuDong();
        string ho, ten, phai;
        int trangThai = 1;

        if (cot.size() >= 5 && laSoNguyen(cot[0])) {
            ho = cot[1];
            ten = cot[2];
            phai = cot[3];
            trangThai = chuanHoaTrangThaiThe(cot[4]);
        } else {
            ho = cot[0];
            ten = cot[1];
            phai = cot[2];
            trangThai = chuanHoaTrangThaiThe(cot[3]);
        }

        if (ho.empty() || ten.empty()) continue;

        NodeDocGia* p = taoNodeDocGia(maThe, ho, ten, phai);
        p->trangThai = 1;

        themDocGiaVaoCay(root, p);
    }

    inFile.close();
    
}

void luuDanhSachDocGia(NodeDocGia* root) {
    ofstream outFile("docgia.txt", ios::out);
    if (!outFile.is_open()) {
        return;
    }

    ghiNodeRaFile_NLR(root, outFile);
    outFile.close();
}

void docFileDauSach() {
    ifstream file("dausach.txt");
    if (!file.is_open()) {
        cout << "Khong mo duoc file dausach.txt\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string ISBN, tenSach, tacGia, theLoai;
        string temp;
        int soTrang, namXB;

        getline(ss, ISBN, ',');
        getline(ss, tenSach, ',');
        getline(ss, temp, ','); soTrang = stoi(temp);
        getline(ss, tacGia, ',');
        getline(ss, temp, ','); namXB = stoi(temp);
        getline(ss, theLoai);

        themDauSachTuFile(ISBN,tenSach,soTrang,tacGia,namXB,theLoai);
    }

    file.close();
    
}
void docFileSach() {
    ifstream file("sach.txt");
    if (!file.is_open()) {
        cout << "Khong mo duoc file sach.txt\n";
        return;
    }

    string line;
    while (getline(file, line))
{
    string ISBN, maSach, trangThaiStr, viTri;

    stringstream ss(line);

    getline(ss, ISBN, ',');
    getline(ss, maSach, ',');
    getline(ss, trangThaiStr, ',');
    getline(ss, viTri);

    ISBN = catKhoangTrang(ISBN);
    maSach = catKhoangTrang(maSach);
    viTri = catKhoangTrang(viTri);

    int trangThai = stoi(trangThaiStr);
	int index = timViTriDauSach(ISBN);

if (index != -1)
{
    NodeSach* p = new NodeSach;

    p->MASACH = maSach;
    p->trangThai = trangThai;
    p->viTri = viTri;
    p->next = NULL;

    if (dsDauSach[index]->dms == NULL)
    {
        dsDauSach[index]->dms = p;
    }
    else
    {
        NodeSach* q = dsDauSach[index]->dms;

        while (q->next != NULL)
            q = q->next;

        q->next = p;
    }
}
}
    file.close();
}
void luuFileDauSach()
{
    ofstream file("dausach.txt");

    if (!file.is_open())
        return;

    for (int i = 0; i < soLuongDauSach; i++)
    {
        DauSach* ds = dsDauSach[i];

        file << ds->ISBN << ","
             << ds->tenSach << ","
             << ds->soTrang << ","
             << ds->tacGia << ","
             << ds->namXB << ","
             << ds->theLoai
             << endl;
    }

    file.close();
}
void luuFileSach()
{
    ofstream file("sach.txt", ios::out);

    if (!file.is_open())
        return;

    for (int i = 0; i < soLuongDauSach; i++)
    {
        DauSach* ds = dsDauSach[i];

        for (NodeSach* p = ds->dms; p != NULL; p = p->next)
        {
            file << ds->ISBN << ", "
                 << p->MASACH << ", "
                 << p->trangThai << ","
                 << p->viTri
                 << endl;
        }
    }

    file.close();
}
// =========================================================

void anConTro() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(h, &info);
    info.bVisible = FALSE;
    SetConsoleCursorInfo(h, &info);
}

void hienConTro() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(h, &info);
    info.bVisible = TRUE;
    SetConsoleCursorInfo(h, &info);
}

void xoaManHinh() {
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD home = {0, 0};

    if (!GetConsoleScreenBufferInfo(h, &csbi)) return;

    cellCount = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(h, ' ', cellCount, home, &count);
    FillConsoleOutputAttribute(h, csbi.wAttributes, cellCount, home, &count);
    SetConsoleCursorPosition(h, home);
}

void xoaVungTrongKhung(int y1, int y2) {
    int left = khung_X + 2;
    int right = khung_X + khung_W - 3;
    string blank(right - left + 1, ' ');

    for (int y = y1; y <= y2; y++) {
        gotoXY(left, y);
        cout << blank;
    }
}

int khung_X = 25;
int khung_Y = 2;
int khung_W = 70;
int khung_H = 18;
int dongCuoiNoiDung = 0;


int toaDoX(int xCu) {
    return khung_X + (xCu - 25);
}

int toaDoY(int yCu) {
    return khung_Y + (yCu - 2);
}

void gotoxyKhung(int xCu, int yCu) {
    gotoXY(toaDoX(xCu), toaDoY(yCu));
}


void drawDoubleBox(int x, int y, int w, int h, string title) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, 11);
    gotoXY(x, y);
    cout << (unsigned char)201;
    for (int i = 1; i < w - 1; i++) cout << (unsigned char)205;
    cout << (unsigned char)187;

    for (int i = 1; i < h - 1; i++) {
        gotoXY(x, y + i);
        cout << (unsigned char)186;
        gotoXY(x + w - 1, y + i);
        cout << (unsigned char)186;
    }

    gotoXY(x, y + h - 1);
    cout << (unsigned char)200;
    for (int i = 1; i < w - 1; i++) cout << (unsigned char)205;
    cout << (unsigned char)188;

    if (h >= 6) {
        SetConsoleTextAttribute(hConsole, 3);
        gotoXY(x + 2, y + h - 3);
        for (int i = 0; i < w - 4; i++) cout << (unsigned char)196;
    }

    if (!title.empty()) {
        int viTriChu = x + (w - (int)title.length() - 6) / 2;
        if (viTriChu < x + 2) viTriChu = x + 2;
        gotoXY(viTriChu, y);
        SetConsoleTextAttribute(hConsole, 14);
        cout << "  " << title << "  ";
    }

    SetConsoleTextAttribute(hConsole, 7);
}

void veKhung(string title) {
    xoaManHinh();
    SetConsoleTitleA("QUAN LY THU VIEN - DO AN CTDL");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    khung_W = (consoleWidth * 3) / 4;
    khung_H = (consoleHeight * 3) / 4;

    if (khung_W < 60 && consoleWidth >= 60) khung_W = 60;
    if (khung_H < 18 && consoleHeight >= 18) khung_H = 18;
    if (khung_W > consoleWidth - 2) khung_W = consoleWidth - 2;
    if (khung_H > consoleHeight - 2) khung_H = consoleHeight - 2;

    khung_X = (consoleWidth - khung_W) / 2;
    khung_Y = (consoleHeight - khung_H) / 2;
    if (khung_Y < 1) khung_Y = 1;

    drawDoubleBox(khung_X, khung_Y, khung_W, khung_H, title);
    dongCuoiNoiDung = khung_Y + 3;
}

void veKhungCao(string title, int hCanDung) {
    xoaManHinh();
    SetConsoleTitleA("QUAN LY THU VIEN - DO AN CTDL");

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    khung_W = (consoleWidth * 3) / 4;
    if (khung_W < 80 && consoleWidth >= 80) khung_W = 80;
    if (khung_W > consoleWidth - 2) khung_W = consoleWidth - 2;

    khung_H = hCanDung;
    if (khung_H < 18) khung_H = 18;

    khung_X = (consoleWidth - khung_W) / 2;
    khung_Y = 1;

    drawDoubleBox(khung_X, khung_Y, khung_W, khung_H, title);
    dongCuoiNoiDung = khung_Y + 3;
}
void dungManHinh() {
    int x = khung_X + 3;
    int dongGachNgang = khung_Y + khung_H - 3;
    int y = dongCuoiNoiDung + 2;

    if (y < khung_Y + 4) y = khung_Y + 4;
    if (y >= dongGachNgang) y = dongGachNgang - 1;

    for (int i = max(khung_Y + 3, y - 1); i <= y; i++) {
        gotoXY(x, i);
        cout << string(khung_W - 6, ' ');
    }

    gotoXY(x, y);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
    cout << "Nhan Enter de tiep tuc...";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    while (_getch() != 13);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void hienThiSachCon() {
    cout << "\n===== DANH SACH SACH CO SAN =====\n";
    for (int i = 0; i < soLuongDauSach; i++) {
        for (NodeSach* p = dsDauSach[i]->dms; p != NULL; p = p->next) {
            if (p->trangThai == 0) {
                cout << p->MASACH 
                     << " | " << dsDauSach[i]->tenSach
                     << " | Vi tri: " << p->viTri << endl;
            }
        }
    }
}
void goiYDocGiaRealtime(NodeDocGia* root, string key, int &count) {

    if (root == NULL || count >= 5 || key.empty())
        return;

    goiYDocGiaRealtime(root->left, key, count);

    string ma = to_string(root->MATHE);

    if (ma.find(key) == 0 && count < 5) {

        gotoXY(toaDoX(30), toaDoY(7 + count));

        cout << root->MATHE
             << " | "
             << root->ho << " "
             << root->ten;

        count++;
    }

    goiYDocGiaRealtime(root->right, key, count);
}
void gotoXY(int x, int y) {
    COORD c;
    c.X = x;
    c.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void inDongMenu(int x_menu, int y, string text, bool selected) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    gotoXY(x_menu, y);

    string line = selected ? "  > " + text + "  " : "    " + text + "  ";
    if ((int)line.length() < khung_W - 10) {
        line += string(khung_W - 10 - line.length(), ' ');
    }

    if (selected) {
        SetConsoleTextAttribute(hConsole, 240);
        cout << line;
        SetConsoleTextAttribute(hConsole, 14);
        gotoXY(x_menu - 3, y);
        cout << (unsigned char)175;
    } else {
        SetConsoleTextAttribute(hConsole, 7);
        cout << line;
        gotoXY(x_menu - 3, y);
        cout << " ";
    }

    SetConsoleTextAttribute(hConsole, 7);
}

int menuUI_pro(string menu[], int n, string title) {
    int choice = 0;
    int oldChoice = -1;

    anConTro();
    veKhung(title);

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    string tenHeThong = "QUAN LY THU VIEN";
    int titleX = khung_X + (khung_W - (int)tenHeThong.length()) / 2;
    gotoXY(titleX, khung_Y + 2);
    SetConsoleTextAttribute(hConsole, 11);
    cout << tenHeThong;

    string subTitle = "HE THONG QUAN LY THU VIEN";
    int subX = khung_X + (khung_W - (int)subTitle.length()) / 2;
    gotoXY(subX, khung_Y + 3);
    SetConsoleTextAttribute(hConsole, 14);
    cout << subTitle;

    int x_menu = khung_X + 8;
    int y_start = khung_Y + 6;

    for (int i = 0; i < n; i++) {
        inDongMenu(x_menu, y_start + i, menu[i], i == choice);
    }

    gotoXY(khung_X + 5, khung_Y + khung_H - 2);
    SetConsoleTextAttribute(hConsole, 8);
    cout << "Mui ten len/xuong: Di chuyen   |   Enter: Chon";
    SetConsoleTextAttribute(hConsole, 7);

    while (true) {
        int key = _getch();

        oldChoice = choice;

        if (key == 224) {
            key = _getch();
            if (key == 72) choice = (choice - 1 + n) % n;
            if (key == 80) choice = (choice + 1) % n;
        }
        else if (key == 13) {
            hienConTro();
            return choice;
        }

        if (oldChoice != choice) {
            inDongMenu(x_menu, y_start + oldChoice, menu[oldChoice], false);
            inDongMenu(x_menu, y_start + choice, menu[choice], true);
        }
    }
}

void printLine(int x, int &y, int w, string text) {
    gotoXY(x, y);
    cout << "| " << text;
    y++;
}
void veKhungLoading(int x, int y, int w, int h)
{
    gotoXY(x, y);             cout << char(218);
    gotoXY(x + w - 1, y);     cout << char(191);
    gotoXY(x, y + h - 1);     cout << char(192);
    gotoXY(x + w - 1, y + h - 1); cout << char(217);
    for (int i = 1; i < w - 1; i++) {
        gotoXY(x + i, y); cout << char(196);
        gotoXY(x + i, y + h - 1); cout << char(196);
    }
    for (int i = 1; i < h - 1; i++) {
        gotoXY(x, y + i); cout << char(179);
        gotoXY(x + w - 1, y + i); cout << char(179);
    }
}
void loadingDots(string text, int x, int y,
                 int repeat = 5, int speed = 50)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    gotoXY(x, y);

    SetConsoleTextAttribute(hConsole, 11);
    cout << left << setw(25) << text;
    SetConsoleTextAttribute(hConsole, 7);

    for (int i = 0; i < repeat; i++) {
        gotoXY(x + 25, y);

        for (int j = 0; j < 3; j++) {
            cout << ".";
            Sleep(speed);
        }

        gotoXY(x + 25, y);
        cout << "   ";
    }

    gotoXY(x + 25, y);

    SetConsoleTextAttribute(hConsole, 10);
    cout << "... OK";
    SetConsoleTextAttribute(hConsole, 7);
}
string nhapMaCoGoiY(NodeDocGia* root, string title) {
    string input = "";
    char c;

    while (kbhit()) _getch(); 

    veKhung(title);

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Nhap ma doc gia: ";

    gotoXY(toaDoX(28), toaDoY(6));
    cout << "--- GOI Y ---";

    gotoXY(toaDoX(28), toaDoY(15));
    cout << "(Enter: Chon | Backspace: Xoa)";

    while (true) {

        gotoXY(toaDoX(46), toaDoY(4));
        cout << "                    ";

        gotoXY(toaDoX(46), toaDoY(4));
        cout << input;

        for (int i = 7; i <= 11; i++) {
            gotoXY(toaDoX(30), toaDoY(i));
            cout << "                                            ";
        }

        int count = 0;
        if (!input.empty()) {
            goiYDocGiaRealtime(root, input, count);
        }

        gotoXY(toaDoX(46 + input.length()), toaDoY(4));

        c = _getch();

        if (c == 13) { 
            break;
        }
        else if (c == 8) { 
            if (!input.empty()) {
                input.pop_back();
            }
        }
        else if (isdigit(c)) { 
            if (input.length() < 10) { 
                input += c;
            }
        }
    }

    return input;
}
int chonMenu(string menu[], int n, string title) {
    int choice = 0;

    while (true) {
        xoaManHinh();

        cout << "=================================\n";
        cout << "        " << title << endl;
        cout << "=================================\n";

        for (int i = 0; i < n; i++) {
            if (i == choice) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE | 15);
                cout << " > " << menu[i] << endl;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            } else {
                cout << "   " << menu[i] << endl;
            }
        }

        int key = _getch();

        if (key == 224) {
            key = _getch();
            if (key == 72) choice = (choice - 1 + n) % n;
            else if (key == 80) choice = (choice + 1) % n;
        } else if (key == 13) {
            return choice;
        }
    }
}

 
void inSachDangMuonCuaDocGiaTrongKhung(NodeDocGia* dg, int dongBatDau, int soDongToiDa) {
    gotoXY(toaDoX(28), toaDoY(dongBatDau));
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "Sach doc gia dang muon:";
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

    int dong = dongBatDau + 1;
    bool co = false;

    if (dg != NULL) {
        for (NodeMuonTra* mt = dg->dsMuonTra; mt != NULL && dong < dongBatDau + 1 + soDongToiDa; mt = mt->next) {
            if (mt->trangThai == 0) {
                co = true;
                int indexDS = -1;
                timCuonSach(mt->MASACH, indexDS);
                string tenSach = (indexDS != -1) ? dsDauSach[indexDS]->tenSach : "Khong ro";
                if ((int)tenSach.length() > 26) tenSach = tenSach.substr(0, 23) + "...";

                gotoXY(toaDoX(30), toaDoY(dong));
                cout << "- " << mt->MASACH << " | " << tenSach
                     << " | Muon: " << ngayThanhChuoi(mt->ngayMuon);
                dong++;
            }
        }
    }

    if (!co) {
        gotoXY(toaDoX(30), toaDoY(dong));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
        cout << "Doc gia khong muon cuon sach nao.";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        dong++;
    }

    dongCuoiNoiDung = toaDoY(dong);
}

int nhapLuaChonBangEnter(int minVal, int maxVal, int dong) {
    string s;
    while (true) {
        gotoXY(toaDoX(28), toaDoY(dong));
        cout << string(khung_W - 8, ' ');
        gotoXY(toaDoX(28), toaDoY(dong));
        cout << "Chon: ";
        getline(cin, s);
        s = catKhoangTrang(s);
        if (laSoNguyen(s)) {
            int x = stoi(s);
            if (x >= minVal && x <= maxVal) return x;
        }
        gotoXY(toaDoX(28), toaDoY(dong + 2));
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        cout << "Loi: Hay nhap " << minVal << " hoac " << maxVal << " roi nhan Enter!";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        _getch();
        gotoXY(toaDoX(28), toaDoY(dong + 2));
        cout << string(khung_W - 8, ' ');
    }
}

int main() {
    NodeDocGia* root = NULL; 
    khoiTaoKhoMaThe();
	system("cls");
	
	int x = 35;
	int y = 10;
	
	veKhungLoading(x, y, 50, 6);
	loadingDots("Dang tai doc gia",  x + 3, y + 1);
	docDanhSachDocGia(root);
	
	loadingDots("Dang tai dau sach", x + 3, y + 2);
	docFileDauSach();
	
	loadingDots("Dang tai sach",     x + 3, y + 3);
	docFileSach();
	SetConsoleOutputCP(437);

    string menuChinh[] = {
    "QUAN LY DOC GIA",
    "QUAN LY SACH",
    "TIM KIEM SACH",
    "THONG KE",
    "THOAT"
	};

	string menuDocGia[] = {
	    "Them doc gia",
	    "Xoa doc gia",
	    "Hieu chinh doc gia",
	    "In danh sach",
	    "Quay lai"
	};
	
	string menuSach[] = {
	    "Them dau sach",
	    "Nhap sach",
	    "In sach",
	    "Muon sach",
	    "Tra sach",
	    "Quay lai"
	};
	while (true) {
    int chinh = menuUI_pro(menuChinh, 5, " MENU CHINH ");

    // ================= DOC GIA =================
    if (chinh == 0) {
    while (true) {
        int dg = menuUI_pro(menuDocGia, 5, " DOC GIA ");

        if (dg == 0) {
            veKhung("THEM DOC GIA");

            while (kbhit()) _getch();

            gotoXY(toaDoX(28), toaDoY(11));
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            cout << "Luu y: Ho, ten chi nhap chu; khong de trong; moi tu cach nhau 1 khoang trang.";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

            cin.clear();
            string ho = nhapChuoiTenHopLe("Ho", 4);
            string ten = nhapChuoiTenHopLe("Ten", 5);
            string phai = nhapPhaiHopLe(6);

            int maMoi = layMaTheTuDong();
            themDocGiaVaoCay(root, taoNodeDocGia(maMoi, ho, ten, phai));
			luuDanhSachDocGia(root);
            gotoXY(toaDoX(28), toaDoY(9));
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            cout << ">>> Them thanh cong! Ma: " << maMoi;
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            dongCuoiNoiDung = toaDoY(9);

            dungManHinh();
        }

        else if (dg == 1) {

		    veKhung("XOA DOC GIA");
		
		    string input = nhapMaCoGoiY(root, "XOA DOC GIA");
		
		    if (input.empty()) {
		        dungManHinh();
		        continue;
		    }
		
		    int ma = stoi(input);
		
		    bool kq = xoaDocGia(root, ma);

		
		
		    gotoXY(toaDoX(28), toaDoY(14));
		
		    if (kq) {
		
		        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
		
		        cout << ">>> Xoa doc gia thanh cong!";
		    }
		    else {
		
		        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
		
		        cout << ">>> Khong the xoa doc gia!";
		    }
		
		    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		
		    dungManHinh();
		}

        else if (dg == 2) {

		    veKhung("HIEU CHINH DOC GIA");
		
		    string input = nhapMaCoGoiY(root, "HIEU CHINH DOC GIA");
		
		    if (input.empty()) {
		        dungManHinh();
		        continue;
		    }	
		    int ma = stoi(input);
			hieuChinhDocGia(root, ma);
			luuDanhSachDocGia(root);
			dungManHinh();
		}
        else if (dg == 3) {
		    veKhung("DANH SACH DOC GIA");
		    gotoXY(toaDoX(28), toaDoY(4));
		    cout << "1. Theo ma";
		
		    gotoXY(toaDoX(28), toaDoY(5));
		    cout << "2. Theo ten";
		
		    int luaChonIn = nhapLuaChonBangEnter(1, 2, 7);
	
		    if (luaChonIn == 1) {
	
		        veKhung("DANH SACH THEO MA");
	
		        int dong = 5;
	
		        inDanhSachTheoMaThe(root, dong);
		    }
		    else {
	
		        veKhung("DANH SACH THEO TEN");
	
		        int dong = 5;
	
		        inDanhSachTheoTenHo(root, dong);
		    }
	
		    dungManHinh();
		}

        else break;
    }
}
    // ================= SACH =================

    else if (chinh == 1) {
    while (true) {
        int s = menuUI_pro(menuSach, 6, " SACH ");

        if (s == 0) {
            veKhung("THEM DAU SACH");

            while (kbhit()) _getch();

            gotoXY(toaDoX(28), toaDoY(3));
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            cout << catTheoKhung("Huong dan nhap ISBN: viet tat cua ten sach in hoa. VD: Cau truc du lieu -> CTDL");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

            int namHienTai = layNgayHeThong().nam;
            string ISBN = nhapISBNHopLe(5);
            string ten = nhapChuoiSachHopLe("Ten     ", 6);
            string tg = nhapChuoiSachHopLe("Tac gia ", 7);
            string tl = nhapChuoiSachHopLe("The loai", 8);
            int trang = nhapSoNguyenDuongHopLe("So trang", 9, 1, 10000);
            int nam = nhapSoNguyenDuongHopLe("Nam XB  ", 10, 1000, namHienTai);

            gotoXY(toaDoX(28), toaDoY(12));
            if (themDauSach(ISBN, ten, trang, tg, nam, tl)) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                cout << "Da them dau sach thanh cong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }
            dongCuoiNoiDung = toaDoY(12);

            dungManHinh();
        }
            else if (s == 1) {

			    xoaManHinh();
			    veKhung("NHAP SACH (THEM SACH CON)");
			
			    while (kbhit()) _getch();
			
			    string ISBN = nhapISBNTheoTenRealtime("CHON DAU SACH DE NHAP SACH");
			
			    xoaManHinh();
			    veKhung("NHAP SACH (THEM SACH CON)");
			
			    if (ISBN.empty()) {
			
			        gotoXY(toaDoX(28), toaDoY(6));
			        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			        cout << "Chua chon dau sach!";
			        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			
			        dongCuoiNoiDung = toaDoY(6);
			    }
			    else {
			
			        int index = timViTriDauSach(ISBN);
			
			        if (index == -1) {
			
			            gotoXY(toaDoX(28), toaDoY(6));
			            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
			            cout << "Khong tim thay ISBN nay!";
			            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			
			            dongCuoiNoiDung = toaDoY(6);
			        }
			        else {
			
			            gotoXY(toaDoX(28), toaDoY(4));
			            cout << "ISBN    : " << dsDauSach[index]->ISBN;
			
			            gotoXY(toaDoX(28), toaDoY(5));
			            cout << "Dau sach: "
			                 << catTheoKhung(dsDauSach[index]->tenSach);
			
			            int soCuon =
			                nhapSoNguyenDuongHopLe(
			                    "So cuon muon them",
			                    7,
			                    1,
			                    100
			                );
			
			            int dong = 9;
			            int daThem = 0;
			
			            for (int i = 0; i < soCuon; i++) {
			
			                string maSachMoi =
			                    layMaSachMoiTheoISBN(ISBN);
			
			                string viTri;
			
			                while (true) {
			
			                    gotoXY(toaDoX(28), toaDoY(dong));
			                    cout << string(khung_W - 8, ' ');
			
			                    gotoXY(toaDoX(28), toaDoY(dong));
			                    SetConsoleTextAttribute(
			                        GetStdHandle(STD_OUTPUT_HANDLE),
			                        8
			                    );
			                    cout << "Huong dan nhap vi tri: Ngan-Ke (VD: A1-B2)";
			                    SetConsoleTextAttribute(
			                        GetStdHandle(STD_OUTPUT_HANDLE),
			                        7
			                    );
			
			                    gotoXY(toaDoX(28), toaDoY(dong + 1));
			                    cout << string(khung_W - 8, ' ');
			
			                    gotoXY(toaDoX(28), toaDoY(dong + 1));
			                    cout << "Vi tri cho "
			                         << maSachMoi
			                         << ": ";
			
			                    getline(cin, viTri);
			                    viTri = chuanHoaViTriNganKe(viTri);
			
			                    if (viTri.empty())
			                        baoLoiNhapDauSach(
			                            "Vi tri khong duoc de trong"
			                        );
			                    else if (!hopLeViTriNganKe(viTri))
			                        baoLoiNhapDauSach(
			                            "Vi tri phai theo dang Ngan-Ke (VD: A1-B2)"
			                        );
			                    else if (viTriDaTonTai(viTri))
			                        baoLoiNhapDauSach(
			                            "Vi tri nay da co sach"
			                        );
			                    else
			                        break;
			                }
			
			                if (themSachVaoDauSach(ISBN, viTri)) {
			
			                    daThem++;
			
			                    gotoXY(toaDoX(28), toaDoY(dong + 2));
			
			                    SetConsoleTextAttribute(
			                        GetStdHandle(STD_OUTPUT_HANDLE),
			                        10
			                    );
			
			                    cout << "Da tao ma "
			                         << maSachMoi
			                         << " tai vi tri "
			                         << viTri
			                         << string(20, ' ');
			
			                    SetConsoleTextAttribute(
			                        GetStdHandle(STD_OUTPUT_HANDLE),
			                        7
			                    );
			
			                    dong += 4;
			                }
			            }
			
			            gotoXY(toaDoX(28), toaDoY(dong + 1));
			
			            SetConsoleTextAttribute(
			                GetStdHandle(STD_OUTPUT_HANDLE),
			                10
			            );
			
			            cout << "Hoan tat: da them "
			                 << daThem
			                 << "/"
			                 << soCuon
			                 << " cuon sach.";
			
			            SetConsoleTextAttribute(
			                GetStdHandle(STD_OUTPUT_HANDLE),
			                7
			            );
			
			            dongCuoiNoiDung = toaDoY(dong + 1);
			        }
			    }
			
			    dungManHinh();
			}

            else if (s == 2) { 
                veKhung("DANH SACH CO TRONG KHO");
                inDanhSachDauSach(); 
                dungManHinh();
            }

        else if (s == 3) {
            string input = nhapMaCoGoiY(root, "MUON SACH");

            veKhung("MUON SACH");

            if (input.empty()) {
                gotoXY(toaDoX(28), toaDoY(4));
                inLoiChucNang("Ma doc gia khong duoc de trong!");
                dungManHinh();
                continue;
            }

            int ma = stoi(input);
            NodeDocGia* dg = timDocGia(root, ma);

            if (dg == NULL) {
                gotoXY(toaDoX(28), toaDoY(4));
                inLoiChucNang("Khong tim thay doc gia!");
                dungManHinh();
                continue;
            }

            gotoXY(toaDoX(28), toaDoY(4));
            cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;

            gotoXY(toaDoX(28), toaDoY(6));
            cout << "Dang muon: " << demSachDangMuon(dg) << "/3";

            inSachDangMuonCuaDocGiaTrongKhung(dg, 8, 5);

            gotoXY(toaDoX(28), toaDoY(14));
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            cout << "Nhan Enter de tiep tuc chon sach muon...";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            while (_getch() != 13);

            if (demSachDangMuon(dg) >= 3) {
                gotoXY(toaDoX(28), toaDoY(14));
                inLoiChucNang("Doc gia da muon toi da 3 cuon!");
                dongCuoiNoiDung = toaDoY(14);
                dungManHinh();
                continue;
            }

            string maSach = nhapMaSachTheoTenRealtime("CHON SACH MUON", true, NULL);

            veKhung("MUON SACH");
            gotoXY(toaDoX(28), toaDoY(4));
            cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;
            gotoXY(toaDoX(28), toaDoY(6));
            cout << "Dang muon: " << demSachDangMuon(dg) << "/3";
            gotoXY(toaDoX(28), toaDoY(8));
            cout << "Ma sach muon: " << maSach;

            int indexDS = -1;
            NodeSach* sach = timCuonSach(maSach, indexDS);

            if (sach != NULL && indexDS != -1) {
                gotoXY(toaDoX(28), toaDoY(10));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                cout << "Sach: " << dsDauSach[indexDS]->tenSach << " | Vi tri: " << sach->viTri << " | TT: ";
                if (sach->trangThai == 0) cout << "Ranh";
                else if (sach->trangThai == 1) cout << "Da muon";
                else cout << "Thanh ly";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }

            gotoXY(toaDoX(28), toaDoY(12));
            muonSach(root, ma, maSach);
            dongCuoiNoiDung = toaDoY(12);

            dungManHinh();
        }
        else if (s == 4) {
            string input = nhapMaCoGoiY(root, "TRA SACH");

            veKhung("TRA SACH");

            if (input.empty()) {
                gotoXY(toaDoX(28), toaDoY(4));
                inLoiChucNang("Ma doc gia khong duoc de trong!");
                dungManHinh();
                continue;
            }

            int ma = stoi(input);
            NodeDocGia* dg = timDocGia(root, ma);

            if (dg == NULL) {
                gotoXY(toaDoX(28), toaDoY(4));
                inLoiChucNang("Khong tim thay doc gia!");
                dungManHinh();
                continue;
            }

            gotoXY(toaDoX(28), toaDoY(4));
            cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;

            inSachDangMuonCuaDocGiaTrongKhung(dg, 5, 7);

            if (demSachDangMuon(dg) == 0) {
                dongCuoiNoiDung = toaDoY(7);
                dungManHinh();
                continue;
            }

            gotoXY(toaDoX(28), toaDoY(14));
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            cout << "Nhan Enter de chon sach can tra...";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            while (_getch() != 13);

            string maSach = nhapMaSachTheoTenRealtime("CHON SACH TRA", false, dg);

            veKhung("TRA SACH");
            gotoXY(toaDoX(28), toaDoY(4));
            cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;
            gotoXY(toaDoX(28), toaDoY(5));
            cout << "Ma sach tra: " << maSach;

            int indexDS = -1;
            NodeSach* sach = timCuonSach(maSach, indexDS);
            if (sach != NULL && indexDS != -1) {
                gotoXY(toaDoX(28), toaDoY(7));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                cout << "Sach: " << dsDauSach[indexDS]->tenSach << " | Vi tri: " << sach->viTri;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }

            gotoXY(toaDoX(28), toaDoY(9));
            traSach(root, ma, maSach);
            dongCuoiNoiDung = toaDoY(9);

            dungManHinh();
        }


        else break;
    }
}
    // ================= TIM KIEM =================

    else if (chinh == 2) {
    while (true) {
        string menuTimKiem[] = {
            "Tim theo ten ",
            "Tim theo the loai",
            "Quay lai"
        };

        int tk = menuUI_pro(menuTimKiem, 3, " TIM KIEM ");

        if (tk == 0) {
            string tuKhoa = nhapTenSachRealtime();
            xoaManHinh();
            timSachTheoTen(tuKhoa);
            dungManHinh();
        }
        else if (tk == 1) {
            string theLoai = nhapTheLoaiRealtime();
				xoaManHinh();    		
				timSachTheoTheLoai(theLoai);	
				dungManHinh();
        }
        else break;
    }
}
    // ================= THONG KE =================
    else if (chinh == 3) {
    while (true) {
        string menuThongKe[] = {
            "Top 10 sach",
            "Sach dang muon",
            "Doc gia qua han",
            "Quay lai"
        };

        int tk = menuUI_pro(menuThongKe, 4, " THONG KE ");

        if (tk == 0) {
            veKhung("TOP 10");
            inTop10SachMuonNhieu();
            dungManHinh();
        }
        else if (tk == 1) {
            veKhung("SACH DANG MUON");
            string input = nhapMaCoGoiY(root, "SACH DANG MUON");
            xoaManHinh();
            veKhung("SACH DANG MUON");
            if (!input.empty()) {
                int ma = stoi(input);
                lietKeSachDangMuon(root, ma);
            } else {
                gotoXY(toaDoX(28), toaDoY(4));
                inLoiChucNang("Ma doc gia khong duoc de trong!");
                dongCuoiNoiDung = toaDoY(4);
            }
            dungManHinh();
        }
        else if (tk == 2) {
            inDanhSachDocGiaQuaHan(root);
            dungManHinh();
        }
        else break;
    }
}

    // ================= THOAT =================
    else if (chinh == 4) {
        veKhung("THOAT CHUONG TRINH");

        ofstream outFile("docgia.txt", ios::out);
        gotoXY(khung_X + 5, khung_Y + 5);

        if (!outFile.is_open()) {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
            cout << ">>> Loi tao file docgia.txt!";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        } else {
            ghiNodeRaFile_NLR(root, outFile);
            outFile.close();
            luuFileDauSach();
			luuFileSach();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
            
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
        }

        gotoXY(khung_X + 5, khung_Y + 7);
        
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        cout << "Cam on ban da su dung chuong trinh.";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        dungManHinh();
        xoaManHinh();
        return 0;
    }
}
}