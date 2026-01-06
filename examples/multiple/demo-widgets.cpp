#include <QtImGui.h>
#include <imgui.h>
#include <implot.h>

#include <cmath> // sin

#include <QApplication>
#include <QGridLayout>
#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
#include <QSurfaceFormat>
#include <QTimer>

class DemoWindow
  : public QOpenGLWidget
  , private QOpenGLExtraFunctions
{
 protected:
  void initializeGL() override
  {
    initializeOpenGLFunctions();
    ref = QtImGui::initialize(this, false);

    // Update at 60 fps
    auto* timer = new QTimer(this);
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(16);

    // create plot context
    ctx = ImPlot::CreateContext();
  }
  void paintGL() override
  {
    QtImGui::newFrame(ref);
    ImPlot::SetCurrentContext(ctx);

    static bool             use_work_area = false;
    static ImGuiWindowFlags flags =
      ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

    // We demonstrate using the full viewport area or the work area (without
    // menu-bars, task-bars etc.) Based on your use case you may want one of the
    // other.
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(use_work_area ? viewport->WorkPos : viewport->Pos);
    ImGui::SetNextWindowSize(use_work_area ? viewport->WorkSize : viewport->Size);

    ImPlotDragToolFlags dragflags = ImPlotDragToolFlags_None;
    if (ImGui::Begin("Example: Fullscreen window", nullptr, flags)) {
      ImGui::BulletText("Click and drag the horizontal and vertical lines.");

      ImGui::Checkbox("Show Labels##1", &show_labels);
      if (ImPlot::BeginPlot("##guides", ImVec2(-1, -1), ImPlotFlags_CanvasOnly)) {
        ImPlot::DragLineX(0, &x1, ImVec4(1,1,1,1),1, dragflags);
        ImPlot::DragLineX(1, &x2, ImVec4(1,1,1,1),1, dragflags);
        ImPlot::DragLineY(2, &y1, ImVec4(1,1,1,1),1, dragflags);
        ImPlot::DragLineY(3, &y2, ImVec4(1,1,1,1),1, dragflags);
        double xs[1000], ys[1000];
        for (int i = 0; i < 1000; ++i) {
          xs[i] = (x2 + x1) / 2 + abs(x2 - x1) * (i / 1000.0f - 0.5f);
          ys[i] = (y1 + y2) / 2 + abs(y2 - y1) / 2 * sin(f * i / 10);
        }
        ImPlot::PlotLine("Interactive Data", xs, ys, 1000);
        // ImPlot::SetAxis(ImAxis_Y2);
        ImPlot::DragLineY(4, &f, ImVec4(1, 0.5f, 1, 1));
        ImPlot::EndPlot();
      }
    }
    ImGui::End();

    // Do render before ImGui UI is rendered
    glViewport(0, 0, width(), height());
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui::Render();  // \TODO should let qtimgui call render
    QtImGui::render(ref);
  }

 private:
  ImVec4             clear_color = ImColor(114, 144, 154);
  QtImGui::RenderRef ref         = nullptr;

  double         x1          = 0.2;
  double         x2          = 0.8;
  double         y1          = 0.25;
  double         y2          = 0.75;
  double         f           = 0.1;
  bool           show_labels = true;
  ImPlotContext* ctx         = nullptr;
};

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);

  // Use OpenGL 3 Core Profile, when available
  QSurfaceFormat glFormat;
  if (QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL) {
    glFormat.setVersion(3, 3);
    glFormat.setProfile(QSurfaceFormat::CoreProfile);
  }
  QSurfaceFormat::setDefaultFormat(glFormat);

  // make four
  auto* w = new QWidget();
  auto* l = new QGridLayout;
  w->setLayout(l);
  l->addWidget(new DemoWindow(), 0, 0);
  l->addWidget(new DemoWindow(), 0, 1);
  l->addWidget(new DemoWindow(), 1, 0);
  l->addWidget(new DemoWindow(), 1, 1);
  w->setWindowTitle("QtImGui multiple widgets example");
  w->resize(1280, 720);
  w->show();

  return a.exec();
}
