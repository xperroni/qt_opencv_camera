#include "CameraWidget.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QCloseEvent>

CameraWidget::CameraWidget(QWidget *parent)
    : QWidget(parent),
      m_label(new QLabel(this)),
      m_timer(new QTimer(this))
{
    m_label->setAlignment(Qt::AlignCenter);
    m_label->setText("Camera not started");

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_label);
    setLayout(layout);
    setWindowTitle("Qt6 + OpenCV Webcam");

    connect(m_timer, &QTimer::timeout, this, &CameraWidget::grabFrame);
}

CameraWidget::~CameraWidget()
{
    stopCamera();
}

bool CameraWidget::startCamera(int deviceIndex)
{
    QMutexLocker locker(&m_mutex);
    if (m_running) return true;

    // Try to open the camera
    if (!m_cap.open(deviceIndex)) {
        qWarning() << "Failed to open camera device" << deviceIndex;
        return false;
    }

    // Set some reasonable properties (optional)
    // m_cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    // m_cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
    // m_cap.set(cv::CAP_PROP_FPS, 30);

    m_running = true;
    m_timer->start(30); // ~33ms -> ~30 FPS
    return true;
}

void CameraWidget::stopCamera()
{
    QMutexLocker locker(&m_mutex);
    if (!m_running) return;

    m_timer->stop();
    if (m_cap.isOpened()) m_cap.release();
    m_running = false;
    m_label->setText("Camera stopped");
}

void CameraWidget::grabFrame()
{
    // Lock while grabbing and converting
    QMutexLocker locker(&m_mutex);
    if (!m_running || !m_cap.isOpened()) return;

    cv::Mat frame;
    if (!m_cap.read(frame) || frame.empty()) {
        qWarning() << "Empty frame or read failed";
        return;
    }

    QImage img = matToQImage(frame);
    if (img.isNull()) return;

    // Scale the label to keep window responsive; preserve aspect ratio
    m_label->setPixmap(QPixmap::fromImage(img).scaled(
        m_label->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

QImage CameraWidget::matToQImage(const cv::Mat &mat)
{
    // Support common types
    switch (mat.type()) {
    case CV_8UC3: {
        // BGR -> RGB
        cv::Mat rgb;
        cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
        // Copy to QImage to own the data
        return QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
    }
    case CV_8UC1: {
        return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8).copy();
    }
    case CV_8UC4: {
        // BGRA -> RGBA (Qt's Format_ARGB32 uses premultiplied alpha; may need conversion depending on use)
        cv::Mat rgba;
        cv::cvtColor(mat, rgba, cv::COLOR_BGRA2RGBA);
        return QImage(rgba.data, rgba.cols, rgba.rows, static_cast<int>(rgba.step), QImage::Format_RGBA8888).copy();
    }
    default:
        qWarning() << "Unsupported cv::Mat type:" << mat.type();
        return QImage();
    }
}

void CameraWidget::closeEvent(QCloseEvent *event)
{
    stopCamera();
    QWidget::closeEvent(event);
}