#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QFileIconProvider>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QtGui/QKeyEvent>
#include <QtGui/QScreen>
#include <yy981/fs.h>

constexpr int ISize(128), LSize(3);
namespace fs = std::filesystem;
QGridLayout layout;
int buttons(0);

class CWidget : public QWidget {
protected:
    void keyPressEvent(QKeyEvent* event) override {
        switch (event->key()) {
            case Qt::Key_Escape:
            case Qt::Key_Enter:
            case Qt::Key_Shift:
                exit(0);  // エスケープまたはエンターで終了
        }
    }
};

void createButton(fs::path f) {
		// std::cout << "[" << f.string() << "]\n";
    // アイコン取得
    QFileInfo fileInfo(QString::fromStdString(f.string()));
    QFileIconProvider iconProvider;
    QIcon fileIcon = iconProvider.icon(fileInfo);
    // アイコンをQPixmapに変換
    QPixmap pixmap = fileIcon.pixmap(ISize, ISize);

    QPushButton* current = new QPushButton(QIcon(pixmap), QString::fromStdString(f.stem().string()));
    current->setIconSize(QSize(ISize, ISize));
    current->setFixedSize(ISize * 1.5, ISize);  // ボタンサイズを固定
    current->setStyleSheet("font-size: " + QString::number(ISize / 5) + "px;font:bold");

    // ボタンがクリックされたときにコマンドを実行
    QObject::connect(current, &QPushButton::clicked, [=] {
		std::cout << f.string() << "\n";
        if (fs::is_directory(f)) yfs::start("callAPP.exe",f.string()); else yfs::start(f.string());
		exit(0);
    });

    layout.addWidget(current, buttons % LSize, buttons / LSize); // グリッドレイアウトに追加
    ++buttons;
}

int main(int argc, char* argv[]) {
	if (argc != 2) proc::start("note.exe","callAPP ARG2 3 10000");
    QApplication a(argc, argv);

    CWidget* window = new CWidget;
    window->setLayout(&layout);

    // ボタンの作成
    for (const fs::directory_entry& e : fs::directory_iterator(std::string(argv[1]))) createButton(e.path());

/*
    // ボタンの数に基づいてウィンドウサイズを変更
    int rows = (buttons + LSize - 1) / LSize; // 必要な行数
    int windowWidth = ISize * LSize * 1.5; // ボタン幅に合わせたウィンドウ幅
    int windowHeight = ISize * rows; // 必要なウィンドウ高さ

    window->resize(windowWidth, windowHeight);  // ウィンドウサイズをボタンに合わせる
*/
    // ウィンドウを画面中央に配置
    QRect screenGeometry = QApplication::primaryScreen()->availableGeometry();
    int x = (screenGeometry.width() - window->width()/2) / 2;
    int y = (screenGeometry.height() - window->height()) / 2;
    window->move(x, y);  // ウィンドウを中央に配置

    window->show();
    return a.exec();
}
