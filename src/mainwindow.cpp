#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "pointmodel.h"
#include "animation.h"
#include "settings.h"

#include <QAction>
#include <QDebug>
#include <QSignalMapper>
#include <QColorDialog>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_animation(new Animation(this))
    , m_pointModel(new PointModel(this))
{
    ui->setupUi(this);


    ui->casteljauWidget->setPointModel(m_pointModel);
    ui->pointView->setModel(m_pointModel);

    readSettings();
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * This function is (and must be) invoked after the canvas class is instantiated, so
 * that default values for all drawing properties are already set and can be read to
 * update the property widgets here accordingly. Of course the mentioned property
 * widgets must also be in place for this to work.
 */
void MainWindow::readSettings()
{
    Settings* settings = Settings::instance();

    settings->enableNotifications(false);
    ui->pointsColorBtn->setColor(settings->value("pointColor").value<QColor>());
    ui->pointsFillBtn->setColor(settings->value("pointFill").value<QColor>());
    ui->bezierCurveColorBtn->setColor(settings->value("bezierCurvePenColor").value<QColor>());
    ui->pointRadius->setValue(settings->value("pointRadius", 1.5).toDouble());
    ui->bezierCurveGroupBox->setChecked(settings->value("drawBezierCurve", true).toBool());
    ui->pointGroupBox->setChecked(settings->value("drawPoints", true).toBool());
    ui->drawIntermediateLines->setChecked(settings->value("drawIntermediateLines", true).toBool());
    ui->connectPointsCheckBox->setChecked(settings->value("connectPoints", true).toBool());
    ui->animationResolutionSpinBox->setValue(settings->value("animationResolution", 0.001).toDouble());
    ui->animationDurationSpinBox->setValue(settings->value("animationDuration", 3500).toInt());
    ui->penWidthSpinBox->setValue(settings->value("bezierCurvePenWidth", 2.0).toDouble());
    settings->enableNotifications(true);

    emit settings->updated(QString(), QVariant());
}

void MainWindow::setupConnections()
{
    connect(ui->progressSlider, &QSlider::valueChanged, [=](){
        qreal progress = (qreal) ui->progressSlider->value() / ui->progressSlider->maximum();
        emit progressChanged(progress);
    });
    connect(this, &MainWindow::progressChanged, ui->casteljauWidget, &CasteljauCanvas::updateProgress);
    connect(ui->action_Quit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionClear, &QAction::triggered, ui->casteljauWidget, &CasteljauCanvas::clear);
    connect(ui->casteljauWidget, &CasteljauCanvas::pointAdded, m_pointModel, &PointModel::appendPoint);
    connect(m_animation, &Animation::finished, [=]() {
        ui->progressSlider->setEnabled(true);
    });

    connect(m_animation, &Animation::valueChanged, [=](QVariant value){
        ui->progressSlider->setValue(value.toInt());
    });

    connect(ui->actionAnimate, &QAction::triggered, [=](){
        ui->progressSlider->setDisabled(true);
        m_animation->setDuration(ui->animationDurationSpinBox->value());
        m_animation->setEasingCurve(QEasingCurve::InOutQuad);
        m_animation->setStartValue(0);
        m_animation->setEndValue(ui->progressSlider->maximum());
        m_animation->start(QAbstractAnimation::KeepWhenStopped);
    });

    // Configuration
    Settings *settings = Settings::instance();

    connect(ui->penWidthSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), [settings](double value) { settings->setValue("bezierCurvePenWidth", value); });
    connect(ui->pointRadius, qOverload<double>(&QDoubleSpinBox::valueChanged), [settings](double value) { settings->setValue("pointRadius", value); });
    connect(ui->bezierCurveGroupBox, &QGroupBox::toggled, [settings](bool on) { settings->setValue("drawBezierCurve", on); });
    connect(ui->pointGroupBox, &QGroupBox::toggled, [settings](bool on) { settings->setValue("drawPoints", on); });
    connect(ui->connectPointsCheckBox, &QCheckBox::toggled, [settings](bool on) { settings->setValue("connectPoints", on); });
    connect(ui->drawIntermediateLines, &QCheckBox::toggled, [settings](bool on) { settings->setValue("drawIntermediateLines", on); });
    connect(ui->animationResolutionSpinBox, qOverload<double>(&QDoubleSpinBox::valueChanged), [settings](double value) { settings->setValue("animationResolution", value); });
    connect(ui->animationDurationSpinBox, qOverload<int>(&QSpinBox::valueChanged), [settings](int value) { settings->setValue("animationDuration", value); });


    QSignalMapper *signalMapper = new QSignalMapper(this);
    QMap<ColorToolButton*, QString> btnKeyMap = {
        { ui->bezierCurveColorBtn, "bezierCurvePenColor" },
        { ui->pointsColorBtn, "pointColor" },
        { ui->pointsFillBtn, "pointFill" }
    };
    for (auto btn: btnKeyMap.keys()) {
        connect(btn, &ColorToolButton::clicked, signalMapper, qOverload<>(&QSignalMapper::map));
        signalMapper->setMapping(btn, btnKeyMap.value(btn));
    }
    connect(signalMapper, qOverload<const QString&>(&QSignalMapper::mappedString), [this, settings, btnKeyMap](const QString& configKey){
        const QColor initialColor = settings->value(configKey, QColor(Qt::white)).value<QColor>();
        const QColor selectedColor = QColorDialog::getColor(initialColor, this, tr("Select Color"));
        if (selectedColor.isValid()) {
            settings->setValue(configKey, selectedColor);
            auto btn = btnKeyMap.key(configKey);
            btn->setColor(selectedColor);
        }
    });
}

