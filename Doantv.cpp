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
string nhapTenSachRealtime() {
    string input = "";
    char c;

    while (kbhit()) _getch();

    anConTro();
    veKhung("TIM KIEM SACH");

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Nhap ten sach: ";

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

        vector<DauSach*> ds = goiYSach(input);
        if (!input.empty()) {
            for (int i = 0; i < ds.size() && i < 7; i++) {
                gotoXY(toaDoX(28), toaDoY(6 + i));
                cout << " - " << ds[i]->tenSach;
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
        return false;
    }

    veKhung("HIEU CHINH DOC GIA");

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string ho, ten, phai;
    string temp;

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Ma the: " << dg->MATHE;

    gotoXY(toaDoX(28), toaDoY(6));
    cout << "Ho moi (" << dg->ho << "): ";
    getline(cin, ho);

    gotoXY(toaDoX(28), toaDoY(7));
    cout << "Ten moi (" << dg->ten << "): ";
    getline(cin, ten);

    gotoXY(toaDoX(28), toaDoY(8));
    cout << "Phai moi (" << dg->phai << "): ";
    getline(cin, phai);

    gotoXY(toaDoX(28), toaDoY(9));
    cout << "Trang thai (0/1): ";
    getline(cin, temp);

    if (!ho.empty()) dg->ho = ho;
    if (!ten.empty()) dg->ten = ten;
    if (!phai.empty()) dg->phai = phai;

    if (!temp.empty()) {
        dg->trangThai = stoi(temp);
    }

    gotoXY(toaDoX(28), toaDoY(11));
    cout << ">>> Cap nhat thanh cong!";

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
    }
}
// --- Các hàm hỗ trợ Xóa Độc giả ---
bool kiemTraDangMuonSach(NodeDocGia* p) {
    NodeMuonTra* k = p->dsMuonTra;
    while (k != NULL) {
        if (k->trangThai == 0 || k->trangThai == 2) return true; 
        k = k->next;
    }
    return false; 
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
			
			cout << "Doc gia dang muon hoac mat sach!";
			
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
void lietKeSachDangMuon(NodeDocGia* root, int maThe) {
    NodeDocGia* dg = timDocGia(root, maThe);

    if (dg == NULL) {
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Khong tim thay doc gia!";
        return;
    }

    gotoXY(toaDoX(28), toaDoY(4));
    cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;

    bool coSach = false;
    int dongIn = 6; 

    for (NodeMuonTra* k = dg->dsMuonTra; k != NULL; k = k->next) {
        if (k->trangThai == 0) { 
            coSach = true;

            int indexDS;
            NodeSach* sach = timCuonSach(k->MASACH, indexDS);
            string tenSach = (indexDS != -1) ? dsDauSach[indexDS]->tenSach : "Khong ro";

            if (tenSach.length() > 25) {
                tenSach = tenSach.substr(0, 22) + "...";
            }

            gotoXY(toaDoX(28), toaDoY(dongIn));
            cout << "- " << k->MASACH << " | " << tenSach;
            dongIn++;
        }
    }

    if (!coSach) {
        gotoXY(toaDoX(28), toaDoY(7));
        cout << "(Doc gia khong muon sach nao)";
    }
}
// =========================================================
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

    int soDauSachKhop = 0;
    for (int i = 0; i < soLuongDauSach; i++) {
        string tl = boDau(dsDauSach[i]->theLoai);
        if (tl.find(key) != string::npos) {
            soDauSachKhop++;
        }
    }

    if (soDauSachKhop == 0) {
        xoaManHinh();
        veKhung("KET QUA TIM THE LOAI");
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Khong tim thay the loai hop le!";
        gotoXY(toaDoX(28), toaDoY(6));
        return;
    }
    int h_dong = (soDauSachKhop * 6) + 4;
    if (h_dong < 12) h_dong = 12;

    xoaManHinh();
    drawDoubleBox(khung_X, khung_Y, khung_W, h_dong, "KET QUA TIM THE LOAI");

    int dongIn = 4;
    for (int i = 0; i < soLuongDauSach; i++) {
        string tl = boDau(dsDauSach[i]->theLoai);

        if (tl.find(key) != string::npos) {
            DauSach* ds = dsDauSach[i];

            gotoXY(toaDoX(28), toaDoY(dongIn++));
            cout << "ISBN    : " << ds->ISBN;
            
            gotoXY(toaDoX(28), toaDoY(dongIn++));
            cout << "Ten     : " << ds->tenSach;
            
            gotoXY(toaDoX(28), toaDoY(dongIn++));
            cout << "Tac gia : " << ds->tacGia;
            
            gotoXY(toaDoX(28), toaDoY(dongIn++));
            cout << "Nam XB  : " << ds->namXB;
            
            gotoXY(toaDoX(28), toaDoY(dongIn++));
            cout << "The loai: " << ds->theLoai;

            gotoXY(toaDoX(28), toaDoY(dongIn++));
            cout << "--------------------------";
        }
    }

    gotoXY(toaDoX(28), toaDoY(dongIn));
}
void timSachTheoTen(string tuKhoa) {
    if (soLuongDauSach == 0) {
        xoaManHinh();
        veKhung("KET QUA TIM KIEM");
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Kho sach rong!";
        return;
    }

    string key = boDau(tuKhoa);
    bool found = false;

    for (int i = 0; i < soLuongDauSach; i++) {
        string ten = boDau(dsDauSach[i]->tenSach);

        if (ten.find(key) != string::npos) {
            found = true;
            DauSach* ds = dsDauSach[i];

            int soSachCon = 0;
            for (NodeSach* p = ds->dms; p != NULL; p = p->next) {
                soSachCon++;
            }

            int h_dong = 4 + 1 + (soSachCon > 0 ? soSachCon : 1) + 4;
            if (h_dong < 12) h_dong = 12; 

            xoaManHinh();
            drawDoubleBox(khung_X, khung_Y, khung_W, h_dong, "KET QUA TIM KIEM");

            gotoXY(toaDoX(28), toaDoY(4));  cout << "ISBN    : " << ds->ISBN;
            gotoXY(toaDoX(28), toaDoY(5));  cout << "Ten     : " << ds->tenSach;
            gotoXY(toaDoX(28), toaDoY(6));  cout << "Tac gia : " << ds->tacGia;
            gotoXY(toaDoX(28), toaDoY(7));  cout << "Nam XB  : " << ds->namXB;
            gotoXY(toaDoX(28), toaDoY(8));  cout << "The loai: " << ds->theLoai;

            gotoXY(toaDoX(28), toaDoY(10));
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); 
            cout << "Danh sach sach con hien co:";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

            int dongIn = 11;
            if (ds->dms == NULL) {
                gotoXY(toaDoX(30), toaDoY(dongIn));
                cout << "(Chua co cuon sach nao trong kho)";
                dongIn++;
            } else {
                for (NodeSach* p = ds->dms; p != NULL; p = p->next) {
                    gotoXY(toaDoX(30), toaDoY(dongIn));
                    cout << "- " << p->MASACH
                         << " | VT: " << p->viTri
                         << " | TT: ";
                    
                    if (p->trangThai == 0) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); 
                        cout << "Ranh";
                    } else if (p->trangThai == 1) {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                        cout << "Da muon";
                    } else {
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8); 
                        cout << "Thanh ly";
                    }
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    dongIn++;
                }
            }

            gotoXY(toaDoX(28), toaDoY(dongIn + 1));
            break; 
        }
    }

    if (!found) {
        xoaManHinh();
        veKhung("KET QUA TIM KIEM");
        gotoXY(toaDoX(28), toaDoY(4));
        cout << "Khong tim thay sach phu hop!";
        gotoXY(toaDoX(28), toaDoY(6));
    }
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
    return true;
}

int demSoLuongSachCon(NodeSach* head) {
    int count = 0;
    for (NodeSach* p = head; p != NULL; p = p->next) count++;
    return count;
}

bool themSachVaoDauSach(string ISBN, string viTri) {
    int index = timViTriDauSach(ISBN);
    if (index == -1) {
        cout << "Loi: Khong tim thay ma ISBN " << ISBN << " trong he thong!" << endl;
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
    
    cout << "Da them thanh cong! Ma sach tu dong: " << maSachMoi << endl;
    return true;
}

void inDanhSachDauSach() {
    int tongDong = 6;

    if (soLuongDauSach > 0) {
        for (int i = 0; i < soLuongDauSach; i++) {
            int soSachCon = 0;
            for (NodeSach* p = dsDauSach[i]->dms; p != NULL; p = p->next) {
                soSachCon++;
            }
            int h = 8 + (soSachCon > 0 ? soSachCon : 1);
            tongDong += h + 1;
        }
    }

    veKhungCao("DANH SACH CO TRONG KHO", tongDong + 4);

    if (soLuongDauSach == 0) {
        gotoXY(khung_X + 3, khung_Y + 3);
        cout << "Kho sach hien tai dang trong!" << endl;
        dongCuoiNoiDung = khung_Y + 4;
        return;
    }

    int x = khung_X + 2;
    int y = khung_Y + 3;
    int w = khung_W - 4;
    int gioiHanDuoi = khung_Y + khung_H - 3;

    for (int i = 0; i < soLuongDauSach; i++) {
        DauSach* ds = dsDauSach[i];

        int soSachCon = 0;
        for (NodeSach* p = ds->dms; p != NULL; p = p->next) {
            soSachCon++;
        }

        int h = 8 + (soSachCon > 0 ? soSachCon : 1);
        if (y + h >= gioiHanDuoi) break;

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
        gotoXY(x, y);
        cout << "+";
        for (int j = 1; j < w - 1; j++) cout << "-";
        cout << "+";

        for (int j = 1; j < h - 1; j++) {
            gotoXY(x, y + j);
            cout << "|";
            gotoXY(x + w - 1, y + j);
            cout << "|";
        }

        gotoXY(x, y + h - 1);
        cout << "+";
        for (int j = 1; j < w - 1; j++) cout << "-";
        cout << "+";

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
        gotoXY(x + 2, y);
        cout << " DAU SACH " << (i + 1) << " ";
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

        gotoXY(x + 3, y + 2);
        cout << "ISBN: " << ds->ISBN;
        gotoXY(x + 3, y + 3);
        cout << "Ten: " << ds->tenSach;
        gotoXY(x + 3, y + 4);
        cout << "Tac gia: " << ds->tacGia;
        gotoXY(x + 3, y + 5);
        cout << "The loai: " << ds->theLoai;

        int dongHienTai = y + 6;
        if (ds->dms != NULL) {
            gotoXY(x + 3, dongHienTai);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
            cout << "-> Cac cuon sach hien co trong kho:";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

            dongHienTai++;
            for (NodeSach* p = ds->dms; p != NULL; p = p->next) {
                if (dongHienTai >= y + h - 1) break;
                gotoXY(x + 6, dongHienTai);
                cout << "+ " << p->MASACH
                     << " | VT: " << p->viTri
                     << " | TT: ";

                if (p->trangThai == 0) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                    cout << "Ranh";
                } else if (p->trangThai == 1) {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                    cout << "Da muon";
                } else {
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
                    cout << "Thanh ly";
                }
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                dongHienTai++;
            }
        } else {
            gotoXY(x + 3, dongHienTai);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
            cout << "-> (Chua co cuon sach nao thuoc dau sach nay)";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            dongHienTai++;
        }

        y += h + 1;
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

bool muonSach(NodeDocGia* root, int maThe, string maSach) {
    NodeDocGia* dg = timDocGia(root, maThe);
    if (dg == NULL) {
        cout << "Loi: Khong tim thay doc gia!" << endl; 
        return false;
    }
    if (dg->trangThai == 0) {
        cout << "Loi: The doc gia dang bi khoa!" << endl; 
        return false;
    }
    
    int soSachDangMuon = 0;
    for (NodeMuonTra* k = dg->dsMuonTra; k != NULL; k = k->next) {
        if (k->trangThai == 2) {
            cout << "Loi: Doc gia dang lam mat sach, khong duoc muon them!" << endl; 
            return false;
        }
        if (k->trangThai == 0) soSachDangMuon++;
    }
    if (soSachDangMuon >= 3) {
        cout << "Loi: Doc gia da muon toi da 3 cuon!" << endl; 
        return false;
    }

    // Kiểm tra định dạng mã sách phải chứa dấu '-'
    if (maSach.find('-') == string::npos) {
        cout << "Loi: Ma sach sai dinh dang (Vi du dung: ISBN-1)!" << endl;
        return false;
    }

    int indexDS = -1;
    NodeSach* sach = timCuonSach(maSach, indexDS);
    if (sach == NULL) {
        cout << "Loi: Khong tim thay ma sach nay trong he thong!" << endl; 
        return false;
    }
    if (sach->trangThai != 0) {
        cout << "Loi: Sach nay da co nguoi muon hoac da thanh ly!" << endl; 
        return false;
    }

    sach->trangThai = 1; 
    dsDauSach[indexDS]->soLuotMuon++; 

    NodeMuonTra* mtMoi = new NodeMuonTra;
    mtMoi->MASACH = maSach;
    mtMoi->ngayMuon = {1, 1, 2024}; 
    mtMoi->trangThai = 0; 
    
    mtMoi->next = dg->dsMuonTra;
    dg->dsMuonTra = mtMoi;

    cout << "=> Muon sach thanh cong!" << endl;
    return true;
}
bool traSach(NodeDocGia* root, int maThe, string maSach) {
    NodeDocGia* dg = timDocGia(root, maThe);
    if (dg == NULL) {
        cout << "Loi: Khong tim thay doc gia!" << endl;
        return false;
    }

    if (maSach.find('-') == string::npos) {
        cout << "Loi: Ma sach sai dinh dang (Vi du dung: ISBN-1)!" << endl;
        return false;
    }

    int indexDS = -1;
    NodeSach* sach = timCuonSach(maSach, indexDS);
    if (sach == NULL) {
        cout << "Loi: Khong tim thay ma sach nay trong he thong!" << endl;
        return false;
    }

    NodeMuonTra* mt = dg->dsMuonTra;
    while (mt != NULL) {
        if (mt->MASACH == maSach && mt->trangThai == 0) {
            mt->trangThai = 1; 
            mt->ngayTra = {15, 1, 2024}; 
            
            sach->trangThai = 0;
            
            cout << "=> Tra sach thanh cong!" << endl;
            return true;
        }
        mt = mt->next;
    }
    cout << "Loi: Doc gia khong muon cuon sach nay!" << endl;
    return false;
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

void docDanhSachDocGia(NodeDocGia* &root) {
    ifstream inFile("docgia.txt");

    if (!inFile.is_open()) {
        cout << "Khong tim thay file docgia.txt!\n";
        return;
    }

    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string ho, ten, phai, temp;
        int trangThai;

        getline(ss, ho, ',');
        getline(ss, ten, ',');
        getline(ss, phai, ',');
        getline(ss, temp, ','); 
        trangThai = stoi(temp);

   
        int maThe = layMaTheTuDong();

        NodeDocGia* p = taoNodeDocGia(maThe, ho, ten, phai);
        p->trangThai = trangThai;

        themDocGiaVaoCay(root, p);
    }

    inFile.close();
    cout << ">>> Da nap doc gia (tu dong cap ma)!\n";
}

void luuDanhSachDocGia(NodeDocGia* root) {
    ofstream outFile("output.txt", ios::out);
    if (!outFile.is_open()) {
        cout << "Loi tao file output.txt" << endl; return;
    }
    ghiNodeRaFile_NLR(root, outFile);
    outFile.close();
    cout << "Da luu du lieu Doc gia vao file output.txt!" << endl;
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

        themDauSach(ISBN, tenSach, soTrang, tacGia, namXB, theLoai);
    }

    file.close();
    cout << ">>> Da load Dau Sach tu file!\n";
}
void docFileSach() {
    ifstream file("sach.txt");
    if (!file.is_open()) {
        cout << "Khong mo duoc file sach.txt\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        string ISBN, viTri;

        stringstream ss(line);
        getline(ss, ISBN, ',');
        getline(ss, viTri);

        themSachVaoDauSach(ISBN, viTri);
    }

    file.close();
    cout << ">> Da load Sach tu file!\n";
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
    int y = khung_Y + khung_H - 2;

    if (dongCuoiNoiDung >= y && dongCuoiNoiDung + 1 < khung_Y + khung_H - 1) {
        y = dongCuoiNoiDung + 1;
    }
    if (y > khung_Y + khung_H - 2) y = khung_Y + khung_H - 2;

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

void loadingDots(string text, int repeat = 3, int speed = 25){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 11);
    cout << text;
    SetConsoleTextAttribute(hConsole, 7);

    for (int i = 0; i < repeat; i++) {
        for (int j = 0; j < 3; j++) {
            cout << ".";
            Sleep(speed);
        }

        Sleep(25);

        cout << "\r" << text << "   " << "\r" << text;
    }

    SetConsoleTextAttribute(hConsole, 10);
    cout << "... OK\n";
    SetConsoleTextAttribute(hConsole, 7);
}
string nhapMaCoGoiY(NodeDocGia* root, string title) {
    string input = "";
    char c;

    // Dọn sạch bộ đệm bàn phím trước khi bắt đầu bắt phím bằng _getch()
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

 
int main() {
    NodeDocGia* root = NULL; 
    khoiTaoKhoMaThe();

	loadingDots("Dang tai doc gia");
	docDanhSachDocGia(root);

	loadingDots("Dang tai dau sach");
	docFileDauSach();

	loadingDots("Dang tai sach");
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

            string ho, ten, phai;

            gotoXY(khung_X + 5,khung_Y +3);
            cout << "Ho   : ";
            
            gotoXY(khung_X + 5,khung_Y +4);
            cout << "Ten  : ";
            
            gotoXY(khung_X + 5,khung_Y +5);
            cout << "Phai : ";

            gotoXY(khung_X + 12, khung_Y + 3);
            getline(cin, ho);
            
            gotoXY(khung_X + 12, khung_Y + 4);
            getline(cin, ten);
            
            gotoXY(khung_X + 12, khung_Y + 5);
            getline(cin, phai);
            
            int maMoi = layMaTheTuDong();
            
            themDocGiaVaoCay(root, taoNodeDocGia(maMoi, ho, ten, phai));
            
            gotoXY(khung_X + 5, khung_Y + 6);
            cout << ">>> Them thanh cong! Ma: " << maMoi;
            
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
		
		    dungManHinh();
		}

        else if (dg == 3) {

		    veKhung("DANH SACH DOC GIA");
		
		    gotoXY(toaDoX(28), toaDoY(4));
		    cout << "1. Theo ma";
		
		    gotoXY(toaDoX(28), toaDoY(5));
		    cout << "2. Theo ten";
		
		    gotoXY(toaDoX(28), toaDoY(7));
		    cout << "Chon: ";
		
		    char c = _getch();
		
		    if (c == '1') {
		
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

            string ISBN = "", ten = "", tg = "", tl = "";
            string sTrang = "", sNam = "";
            int trang = 0, nam = 0;

            while (true) {
                gotoXY(toaDoX(28), toaDoY(4));  cout << "ISBN    :                     "; 
                gotoXY(toaDoX(38), toaDoY(4));  getline(cin, ISBN);

                ISBN.erase(0, ISBN.find_first_not_of(" \t"));
                ISBN.erase(ISBN.find_last_not_of(" \t") + 1);

                if (!ISBN.empty()) break;

                gotoXY(toaDoX(28), toaDoY(11));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
                cout << "Loi: ISBN khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(11)); cout << "                                                 "; 
            }

            while (true) {
                gotoXY(toaDoX(28), toaDoY(5));  cout << "Ten     :                     ";
                gotoXY(toaDoX(38), toaDoY(5));  getline(cin, ten);
                
                ten.erase(0, ten.find_first_not_of(" \t"));
                ten.erase(ten.find_last_not_of(" \t") + 1);

                if (!ten.empty()) break;

                gotoXY(toaDoX(28), toaDoY(11));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: Ten sach khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(11)); cout << "                                                 ";
            }

            while (true) {
                gotoXY(toaDoX(28), toaDoY(6));  cout << "Tac gia :                     ";
                gotoXY(toaDoX(38), toaDoY(6));  getline(cin, tg);
                
                tg.erase(0, tg.find_first_not_of(" \t"));
                tg.erase(tg.find_last_not_of(" \t") + 1);

                if (!tg.empty()) break;

                gotoXY(toaDoX(28), toaDoY(11));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: Tac gia khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(11)); cout << "                                                 ";
            }

            while (true) {
                gotoXY(toaDoX(28), toaDoY(7));  cout << "The loai:                     ";
                gotoXY(toaDoX(38), toaDoY(7));  getline(cin, tl);
                
                tl.erase(0, tl.find_first_not_of(" \t"));
                tl.erase(tl.find_last_not_of(" \t") + 1);

                if (!tl.empty()) break;

                gotoXY(toaDoX(28), toaDoY(11));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: The loai khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(11)); cout << "                                                 ";
            }

            while (true) {
                gotoXY(toaDoX(28), toaDoY(8));  cout << "So trang:                     ";
                gotoXY(toaDoX(38), toaDoY(8));  getline(cin, sTrang);
                
                sTrang.erase(0, sTrang.find_first_not_of(" \t"));
                sTrang.erase(sTrang.find_last_not_of(" \t") + 1);

                if (!sTrang.empty()) {
                    try {
                        trang = stoi(sTrang);
                        if (trang > 0) break;
                    } catch (...) {}
                }

                gotoXY(toaDoX(28), toaDoY(11));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: So trang hop le khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(11)); cout << "                                                 ";
            }

            while (true) {
                gotoXY(toaDoX(28), toaDoY(9));  cout << "Nam XB  :                     ";
                gotoXY(toaDoX(38), toaDoY(9));  getline(cin, sNam);
                
                sNam.erase(0, sNam.find_first_not_of(" \t"));
                sNam.erase(sNam.find_last_not_of(" \t") + 1);

                if (!sNam.empty()) {
                    try {
                        nam = stoi(sNam);
                        if (nam > 0) break;
                    } catch (...) {}
                }

                gotoXY(toaDoX(28), toaDoY(11));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: Nam xuat ban hop le khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(11)); cout << "                                                 ";
            }

            gotoXY(toaDoX(28), toaDoY(11));
            if (themDauSach(ISBN, ten, trang, tg, nam, tl)) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); 
                cout << "Da them dau sach thanh cong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }

            dungManHinh();
        }
		else if (s == 1) { 
                veKhung("NHAP SACH (THEM SACH CON)");

                while (kbhit()) _getch(); 

                string ISBN = "", viTri = "";

                gotoXY(toaDoX(28), toaDoY(4));  cout << "Nhap ma ISBN: ";
                getline(cin, ISBN);

                ISBN.erase(0, ISBN.find_first_not_of(" \t"));
                ISBN.erase(ISBN.find_last_not_of(" \t") + 1);

                if (ISBN.empty()) {
                    gotoXY(toaDoX(28), toaDoY(6));
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
                    cout << "Loi: Ma ISBN khong duoc de trong!";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                } 
                else {
                    int index = timViTriDauSach(ISBN);
                    if (index == -1) {
                        gotoXY(toaDoX(28), toaDoY(6));
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
                        cout << "Loi: Khong tim thay dau sach co ISBN nay!";
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    } 
                    else {
                        gotoXY(toaDoX(28), toaDoY(5)); 
                        cout << "Dau sach: " << dsDauSach[index]->tenSach;

                        gotoXY(toaDoX(28), toaDoY(7));  cout << "Nhap vi tri dat sach: ";
                        getline(cin, viTri);

                        gotoXY(toaDoX(28), toaDoY(9));

                        if (themSachVaoDauSach(ISBN, viTri)) {
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
                            cout << "Nhap sach moi vao kho thanh cong!";
                            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                        }
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
            string input = "";
            int ma = -1;
            NodeDocGia* dg = NULL;

            while (true) {
                veKhung("MUON SACH");
                hienThiSachCon(); 

                input = nhapMaCoGoiY(root, "MUON SACH");
                
                if (input.empty()) {
                    gotoXY(toaDoX(28), toaDoY(13));
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
                    cout << "Loi: Ma doc gia khong duoc de trong!";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    _getch();
                    continue; 
                }

                ma = stoi(input);
                dg = timDocGia(root, ma);

                if (dg == NULL) {
                    gotoXY(toaDoX(28), toaDoY(13));
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
                    cout << "Loi: Khong tim thay doc gia " << ma << "!";
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                    _getch();
                    continue; 
                }

                break; 
            }

            veKhung("MUON SACH");
            gotoXY(toaDoX(28), toaDoY(4));
            cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;

            string maSach = "";
            while (true) {
                gotoXY(toaDoX(28), toaDoY(6));
                cout << "Nhap ma sach muon:                   ";
                gotoXY(toaDoX(48), toaDoY(6));
                getline(cin, maSach);

                maSach.erase(0, maSach.find_first_not_of(" \t"));
                maSach.erase(maSach.find_last_not_of(" \t") + 1);

                if (!maSach.empty()) {
                    break;
                }

                gotoXY(toaDoX(28), toaDoY(8));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: Ma sach khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(8));
                cout << "                                                 ";
            }

            int indexDS = -1;
            NodeSach* sach = timCuonSach(maSach, indexDS);

            if (sach == NULL || indexDS == -1) {
                
                gotoXY(toaDoX(28), toaDoY(8));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
                cout << "Loi: Khong tim thay thong tin ma sach nay!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            } 
            else {
                gotoXY(toaDoX(28), toaDoY(8));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); 
                cout << "Sach: " << dsDauSach[indexDS]->tenSach << " | Vi tri: " << sach->viTri;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);

                gotoXY(toaDoX(28), toaDoY(10));
                muonSach(root, ma, maSach);
            }

            dungManHinh();
        }
        else if (s == 4) {
            veKhung("TRA SACH");

            string input = "";
            while (true) {
                input = nhapMaCoGoiY(root, "TRA SACH");
                if (!input.empty()) {
                    break;
                }
                gotoXY(toaDoX(28), toaDoY(13));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: Ma doc gia khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
            }

            int ma = stoi(input);
            NodeDocGia* dg = timDocGia(root, ma);

            veKhung("TRA SACH");
            gotoXY(toaDoX(28), toaDoY(4));
            if (dg != NULL) {
                cout << "Doc gia: " << dg->MATHE << " - " << dg->ho << " " << dg->ten;
            } else {
                cout << "Ma doc gia: " << ma << " (Khong tim thay!)";
            }

            string maSach = "";
            while (true) {
                gotoXY(toaDoX(28), toaDoY(6));
                cout << "Nhap ma sach tra:                   ";
                gotoXY(toaDoX(46), toaDoY(6));
                getline(cin, maSach);

                maSach.erase(0, maSach.find_first_not_of(" \t"));
                maSach.erase(maSach.find_last_not_of(" \t") + 1);

                if (!maSach.empty()) {
                    break;
                }

                gotoXY(toaDoX(28), toaDoY(8));
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
                cout << "Loi: Ma sach khong duoc de trong!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
                _getch();
                gotoXY(toaDoX(28), toaDoY(8));
                cout << "                                                 ";
            }


            int indexDS = -1;
            NodeSach* sach = timCuonSach(maSach, indexDS);

            gotoXY(toaDoX(28), toaDoY(8));
            if (sach != NULL && indexDS != -1) {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); 
                cout << "Sach: " << dsDauSach[indexDS]->tenSach << " | Vi tri: " << sach->viTri;
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            } else {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); 
                cout << "Loi: Khong tim thay thong tin ma sach nay!";
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
            }

            gotoXY(toaDoX(28), toaDoY(10));
            traSach(root, ma, maSach);

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

            timSachTheoTen(tuKhoa);

            dungManHinh();
        }
        else if (tk == 1) {
            string theLoai = nhapTheLoaiRealtime();

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
            "Quay lai"
        };

        int tk = menuUI_pro(menuThongKe, 3, " THONG KE ");

        if (tk == 0) {
            veKhung("TOP 10");
            inTop10SachMuonNhieu();
            dungManHinh();
        }
        else if (tk == 1) {
            veKhung("SACH DANG MUON");
            string input = nhapMaCoGoiY(root, "SACH DANG MUON");
            if (!input.empty()) {
                int ma = stoi(input);
                
                xoaManHinh();
                veKhung("SACH DANG MUON");

                lietKeSachDangMuon(root, ma);
            }
            dungManHinh();
        }
        else break;
    }
}

    // ================= THOAT =================
    else if (chinh == 4) {
        luuDanhSachDocGia(root);
        break;
    }
}
}