#include "scoreinputdialog.h"
#include <QMessageBox>
// 移除 QRegExpValidator 头文件包含

ScoreInputDialog::ScoreInputDialog(Student* student, QWidget *parent)
    : QDialog(parent)
    , m_inputStudent(new Student())
    , m_isModify(student != nullptr)
{
    // 初始化科目名称（与成绩输入框顺序对应）
    m_subjectNames[0] = "高等数学";
    m_subjectNames[1] = "大学物理";
    m_subjectNames[2] = "程序设计";
    m_subjectNames[3] = "数据结构";
    m_subjectNames[4] = "线性代数";
    m_subjectNames[5] = "离散数学";

    // 修改模式：使用传入的学生对象
    if (m_isModify) {
        delete m_inputStudent;
        m_inputStudent = student;
    }

    initUI();
    // 修改模式：填充已有数据
    if (m_isModify) {
        fillStudentData(student);
        setWindowTitle("修改学生成绩");
    } else {
        setWindowTitle("新增学生成绩录入");
    }

    resize(500, 400);
    setFixedSize(width(), height());
}

void ScoreInputDialog::initUI()
{
    QGridLayout* gridLayout = new QGridLayout(this);
    gridLayout->setSpacing(15);
    gridLayout->setContentsMargins(30, 20, 30, 20);

    // 基础信息行
    gridLayout->addWidget(new QLabel("学号*："), 0, 0, Qt::AlignRight);
    m_leNumber = new QLineEdit(this);
    gridLayout->addWidget(m_leNumber, 0, 1);

    gridLayout->addWidget(new QLabel("姓名*："), 1, 0, Qt::AlignRight);
    m_leName = new QLineEdit(this);
    gridLayout->addWidget(m_leName, 1, 1);

    gridLayout->addWidget(new QLabel("性别*："), 2, 0, Qt::AlignRight);
    m_leSex = new QLineEdit(this);
    m_leSex->setPlaceholderText("男/女");
    gridLayout->addWidget(m_leSex, 2, 1);

    gridLayout->addWidget(new QLabel("专业*："), 3, 0, Qt::AlignRight);
    m_leMajor = new QLineEdit(this);
    gridLayout->addWidget(m_leMajor, 3, 1);

    gridLayout->addWidget(new QLabel("班级*："), 4, 0, Qt::AlignRight);
    m_leClass = new QLineEdit(this);
    gridLayout->addWidget(m_leClass, 4, 1);

    // 成绩输入行（移除QRegExpValidator相关代码）
    for (int i = 0; i < 6; ++i) {
        gridLayout->addWidget(new QLabel(m_subjectNames[i] + "*："), 5 + i, 0, Qt::AlignRight);
        m_leScores[i] = new QLineEdit(this);
        m_leScores[i]->setPlaceholderText("0-100（数字）");
        // 删掉这两行：不再使用正则验证器
        // QRegExp regExp("^[0-9]+(\\.[0-9]{1,2})?$");
        // m_leScores[i]->setValidator(new QRegExpValidator(regExp, this));
        gridLayout->addWidget(m_leScores[i], 5 + i, 1);
    }

    // 按钮行
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnConfirm = new QPushButton("确认录入", this);
    QPushButton* btnCancel = new QPushButton("取消", this);
    btnLayout->addWidget(btnConfirm);
    btnLayout->addWidget(btnCancel);
    gridLayout->addLayout(btnLayout, 11, 0, 1, 2, Qt::AlignCenter);

    connect(btnConfirm, &QPushButton::clicked, this, &ScoreInputDialog::onConfirmClicked);
    connect(btnCancel, &QPushButton::clicked, this, &ScoreInputDialog::onCancelClicked);
}

void ScoreInputDialog::fillStudentData(Student* student)
{
    m_leNumber->setText(student->number);
    m_leNumber->setReadOnly(true); // 学号不可改
    m_leName->setText(student->name);
    m_leSex->setText(student->sex);
    m_leMajor->setText(student->major);
    m_leClass->setText(student->classname);

    for (int i = 0; i < 6; ++i) {
        m_leScores[i]->setText(QString::number(student->scores[i]));
    }
}

QString ScoreInputDialog::validateInput()
{
    // 1. 基础信息非空校验
    if (m_leNumber->text().trimmed().isEmpty()) return "学号不能为空！";
    if (m_leName->text().trimmed().isEmpty()) return "姓名不能为空！";
    if (m_leSex->text().trimmed().isEmpty()) return "性别不能为空！";
    if (m_leMajor->text().trimmed().isEmpty()) return "专业不能为空！";
    if (m_leClass->text().trimmed().isEmpty()) return "班级不能为空！";

    // 2. 成绩校验（强化：检查是否为有效数字 + 范围0-100）
    for (int i = 0; i < 6; ++i) {
        QString scoreText = m_leScores[i]->text().trimmed();
        if (scoreText.isEmpty()) {
            return m_subjectNames[i] + "成绩不能为空！";
        }

        // 检查是否为有效数字（核心修改）
        bool isNumber = false;
        double score = scoreText.toDouble(&isNumber);
        if (!isNumber) { // 如果转换失败（非数字）
            return m_subjectNames[i] + "成绩必须是数字！";
        }

        // 检查范围
        if (score < 0 || score > 100) {
            return m_subjectNames[i] + "成绩需在0-100之间！";
        }
    }

    return ""; // 校验通过
}

void ScoreInputDialog::onConfirmClicked()
{
    QString errMsg = validateInput();
    if (!errMsg.isEmpty()) {
        QMessageBox::warning(this, "输入错误", errMsg);
        return;
    }

    // 封装数据
    m_inputStudent->number = m_leNumber->text().trimmed();
    m_inputStudent->name = m_leName->text().trimmed();
    m_inputStudent->sex = m_leSex->text().trimmed();
    m_inputStudent->major = m_leMajor->text().trimmed();
    m_inputStudent->classname = m_leClass->text().trimmed();

    m_inputStudent->totalScore = 0;
    for (int i = 0; i < 6; ++i) {
        m_inputStudent->scores[i] = m_leScores[i]->text().toDouble();
        m_inputStudent->totalScore += m_inputStudent->scores[i];
    }
    m_inputStudent->averageScore = m_inputStudent->totalScore / 6;

    accept();
}

void ScoreInputDialog::onCancelClicked()
{
    reject();
}
