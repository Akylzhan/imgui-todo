#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>

#include "date_utilities.h"
#include "imgui.h"

struct Date {
  int y, m, d;
  std::string str_m = "";

  Date(int year = 0, int month = 0, int day = 0)
  {
    y = year;
    m = month;
    d = day;
  }

  Date(CurrentYearMonthDay_return a)
  {
    y = a.r0;
    m = a.r1;
    d = a.r2;
  }

  Date(ShowDate_return a)
  {
    y = a.r0;
    str_m = a.r1;
    d = a.r2;
  }

  std::string String()
  {
    if (m == 0) {
      return std::to_string(y) + " " + str_m + " " +
           std::to_string(d);
    }
    return std::to_string(y) + " " + std::to_string(m) + " " +
           std::to_string(d);
  }
};

class ToDoList
{
public:
  typedef std::pair<std::string, bool> task;
  typedef std::map<std::string, std::list<task>> day_to_list;

  ToDoList(ImGuiIO& io)
      : CurrentDayFont(io.Fonts->AddFontFromFileTTF("Cousine-Bold.ttf", 28.0f)),
        DayFont(io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 24.0f)),
        TaskFont(io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 18.0f))
  {

// TODO change return of ShowDate to (int, int, int)
// since map is ordered and string month is shit
    for (int i = 0; i < 50; ++i) {
      Date cd = ShowDate(current_day.y, current_day.m, current_day.d + i);
      std::string day = std::to_string(cd.d) + " " + cd.str_m;
      db[day] = {{"hello", 0}, {"poka", 1}};
      buffers[day] = "";
    }

  }

  ~ToDoList()
  {
  }

  void Render()
  {
    ImGui::PushFont(CurrentDayFont);
    ImGui::PushStyleColor(ImGuiCol_Text, CurrentDayFontColor);
    current_day = CurrentYearMonthDay();
    Date cd = ShowDate(current_day.y, current_day.m, current_day.d);
    ImGui::Text("%s", ("Today is: " + cd.String()).c_str());
    ImGui::PopStyleColor();
    ImGui::PopFont();

    for (auto& [day, tasks] : db) {
      ImGui::Separator();
      RenderDay(day, tasks);
    }
  }

  void RenderDay(const std::string& day, std::list<task>& tasks)
  {
    ImGui::PushFont(DayFont);
    ImGui::PushStyleColor(ImGuiCol_Text, DayFontColor);

    ImGui::Text("%s", day.c_str());
    ImGui::Separator();

    for (auto& task : tasks) {
      RenderTask(task, day);
    }
    NewTaskQuery(day, tasks);

    ImGui::PopStyleColor();
    ImGui::PopFont();
  }

  void RenderTask(task& t, const std::string& id)
  {
    auto& task_content = t.first;

    ImGui::PushStyleColor(ImGuiCol_Text, TaskColor);
    ImGui::PushFont(TaskFont);

    Spacing(30.0f, 10.0f);
    // ImGui::TextWrapped("%s", task_content.c_str());
    std::string str = task_content + "##" + id;
    ImGui::Checkbox(str.c_str(), &t.second);
    ImGui::Separator();

    ImGui::PopStyleColor();
    ImGui::PopFont();
  }

  void NewTaskQuery(const std::string& id, std::list<task>& tasks)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, TaskColor);
    ImGui::PushFont(TaskFont);

    Spacing(30.0f, 10.0f);

    std::string str = "##" + id;

    char* buf = new char[65];
    std::copy(buffers[id].begin(), buffers[id].end(), buf);
    buf[buffers.size()] = '\n';

    bool t = ImGui::InputText(
        str.c_str(), buf, 64, ImGuiInputTextFlags_EnterReturnsTrue);

    buffers[id] = std::string(buf);
    delete[] buf;

    ImGui::SameLine();
    str = "Add" + str;
    if (ImGui::Button(str.c_str())) {
      t = true;
    }

    std::string task = buffers[id];
    if (t && task.length() > 0) {
      tasks.push_back({task, false});
      buffers[id] = "";
    }
    ImGui::PopStyleColor();
    ImGui::PopFont();
  }

private:
  void Spacing(float w, float h)
  {
    ImGui::Dummy(ImVec2(w, h));
    ImGui::SameLine();
  }

  std::map<std::string, std::string> buffers;

  day_to_list db;
  ImFont* CurrentDayFont;
  ImFont* DayFont;
  ImFont* TaskFont;

  Date current_day = CurrentYearMonthDay();

  const ImVec4 CurrentDayFontColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
  const ImVec4 DayFontColor = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
  const ImVec4 TaskColor = (ImVec4)ImColor::HSV(0.5f, 0.7f, 0.7f);
};