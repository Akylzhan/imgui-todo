#pragma once

#include <list>
#include <map>
#include <string>
#include <utility>

#include "imgui.h"

class WeeksList
{
public:
  typedef std::pair<std::string, bool> task;
  typedef std::map<int, std::list<task>> day_to_list;
  typedef std::map<int, day_to_list> weeks;

  WeeksList(ImGuiIO& io)
      : WeekFont(io.Fonts->AddFontFromFileTTF("Cousine-Bold.ttf", 28.0f)),
        DayFont(io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 24.0f)),
        TaskFont(io.Fonts->AddFontFromFileTTF("Cousine-Regular.ttf", 18.0f))
  {
    for (int i = 0; i < NUM_OF_BUFFERS; ++i) {
      buffers[i] = (char*)malloc(64);
    }

    for (int i = 1; i <= 2; ++i) {
      db[i] = {{i,
                {
                    {"hello", false},
                    {"poka", true},
                }}};
    }
  }

  ~WeeksList()
  {
    for (int i = 0; i < NUM_OF_BUFFERS; ++i) {
      free(buffers[i]);
    }
  }

  void Render()
  {
    ImGui::PushFont(WeekFont);
    ImGui::PushStyleColor(ImGuiCol_Text, WeekFontColor);
    for (auto& [week_number, days] : db) {
      ImGui::Text("Week %i", week_number);
      ImGui::Separator();

      for (auto& [day_number, tasks] : days) {
        RenderDay(day_number, tasks);
      }
    }
    ImGui::PopStyleColor();
    ImGui::PopFont();
  }

  void RenderDay(const int& day, std::list<task>& tasks)
  {
    ImGui::PushFont(DayFont);
    ImGui::PushStyleColor(ImGuiCol_Text, DayFontColor);

    Spacing(30.0f, 10.0f);
    ImGui::Text("%i September", day);
    ImGui::Separator();

    for (auto& task : tasks) {
      RenderTask(task, day);
    }
    NewTaskQuery(day, tasks);

    ImGui::PopStyleColor();
    ImGui::PopFont();
  }

  void RenderTask(task& t, const int& id)
  {
    auto& task_content = t.first;

    ImGui::PushStyleColor(ImGuiCol_Text, TaskColor);
    ImGui::PushFont(TaskFont);

    Spacing(80.0f, 10.0f);
    // ImGui::TextWrapped("%s", task_content.c_str());
    auto str = task_content + "##" + std::to_string(id);
    ImGui::Checkbox(str.c_str(), &t.second);
    ImGui::Separator();

    ImGui::PopStyleColor();
    ImGui::PopFont();
  }

  void NewTaskQuery(const int& id, std::list<task>& tasks)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, TaskColor);
    ImGui::PushFont(TaskFont);

    Spacing(80.0f, 10.0f);

    auto str = "##" + std::to_string(id);
    bool t = ImGui::InputText(str.c_str(), buffers[id], 64, ImGuiInputTextFlags_EnterReturnsTrue);

    ImGui::SameLine();
    str = "Add" + str;
    if (ImGui::Button(str.c_str())) {
      t = true;
    }

    std::string task = std::string(buffers[id]);
    if (t && task.length() > 0) {
      tasks.push_back({task, false});
      memset(buffers[id], 0x00, 64);
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
  const int NUM_OF_BUFFERS = 105; // current day - start of semester is the ID
  char* buffers[105];

  weeks db;
  ImFont* WeekFont;
  ImFont* DayFont;
  ImFont* TaskFont;

  const ImVec4 WeekFontColor = ImVec4(1.0f, 0.4f, 0.4f, 1.0f);
  const ImVec4 DayFontColor = ImVec4(1.0f, 0.8f, 0.6f, 1.0f);
  const ImVec4 TaskColor = (ImVec4)ImColor::HSV(0.5f, 0.7f, 0.7f);
};