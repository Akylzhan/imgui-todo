#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>
#include <tuple>

#include <imgui.h>

#include "date_utilities.h"


// TODO: remove repeated code, :67, :89

struct Date {
  int y, m, d;

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
    m = a.r1;
    d = a.r2;
  }

  std::string String()
  {
    return std::to_string(y) + " " + std::to_string(m) + " " +
           std::to_string(d);
  }

};

bool operator<(const Date& l, const Date& r) {
  return std::make_tuple(l.y, l.m, l.d) < 
         std::make_tuple(r.y, r.m, r.d);
}

class ToDoList
{
public:
  typedef std::pair<std::string, bool> task;
  typedef std::map<Date, std::list<task>> day_to_list;

  ToDoList(ImGuiIO& io)
      : CurrentDayFont(io.Fonts->AddFontFromFileTTF("Cousine-Bold.ttf", 28.0f)),
        DayFont(io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 24.0f)),
        TaskFont(io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 18.0f))
  {

    for (int i = 0; i < 50; ++i) {
      Date cd = ShowDate(current_day.y, current_day.m, current_day.d + i);
      db[cd] = {{"hello", 0}, {"poka", 1}};
      std::string day_str = std::to_string(cd.d) + " " + std::to_string(cd.m);
      buffers[day_str] = "";
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
      std::string day_str = std::to_string(day.d) + " " + std::to_string(day.m);
      RenderDay(day_str, tasks);
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

    // TODO change this
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