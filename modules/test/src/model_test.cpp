// SPDX-FileCopyrightText: 2022 Daniel Valcour <fosssweeper@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

/*
 * Copyright (c) 2022 Daniel Valcour
 *
 * This file is part of FossSweeper.
 *
 * FossSweeper is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * FossSweeper is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with FossSweeper. If not,
 * see <https://www.gnu.org/licenses/>.
 *
 */

#include <catch2/catch_all.hpp>
#include <fsweep/Model.hpp>

SCENARIO("A Model is constructed with its default constructor")
{
  GIVEN("A Model constructed with its default constructor")
  {
    const fsweep::Model model;

    THEN("The questions are not enabled") { CHECK(model.GetQuestionsEnabled() == false); }

    THEN("The flag count is 0") { CHECK(model.GetFlagCount() == 0); }

    THEN("There are bomb count bombs left")
    {
      CHECK(model.GetBombsLeft() == fsweep::GameConfiguration::BEGINNER_BOMB_COUNT);
    }

    THEN("There is a beginner difficulty amount of buttons left")
    {
      CHECK(model.GetButtonsLeft() == fsweep::GameConfiguration::BEGINNER_BUTTONS_WIDE *
                                          fsweep::GameConfiguration::BEGINNER_BUTTONS_TALL);
    }

    THEN("The GameState is None") { CHECK(model.GetGameState() == fsweep::GameState::None); }

    THEN("The GameDifficulty is Default (Beginner)")
    {
      CHECK(model.GetGameConfiguration().GetGameDifficulty() == fsweep::GameDifficulty::Default);
    }
  }
}

SCENARIO("A Model is constructed with its overloaded constructor")
{
  GIVEN("A Model constructed with default properties")
  {
    const fsweep::Model model(fsweep::GameConfiguration(8, 8, 2), true, fsweep::GameState::Cool, 15,
                              "dbxfcqr."
                              "........"
                              "........"
                              "........"
                              "........"
                              "........"
                              "........"
                              "........");

    THEN("The GameDifficulty is Custom (8x8) 2b")
    {
      CHECK(model.GetGameConfiguration().GetGameDifficulty() == fsweep::GameDifficulty::Custom);
      CHECK(model.GetGameConfiguration().GetButtonsWide() == 8);
      CHECK(model.GetGameConfiguration().GetButtonsTall() == 8);
      CHECK(model.GetGameConfiguration().GetBombCount() == 2);
    }

    THEN("The amount of Button objects in the buttons vector is correct")
    {
      CHECK(model.GetButtons().size() == 8 * 8);
    }

    THEN("The questions are enabled") { CHECK(model.GetQuestionsEnabled() == true); }

    THEN("The GameState is Cool") { CHECK(model.GetGameState() == fsweep::GameState::Cool); }

    THEN("The game time is 15") { CHECK(model.GetGameTime() == 15); }

    THEN("The Button at (0, 0) is Down state with no bomb")
    {
      CHECK(model.GetButton(0, 0).GetButtonState() == fsweep::ButtonState::Down);
      CHECK(model.GetButton(0, 0).GetHasBomb() == false);
    }

    THEN("The Button at (1, 0) is None state with a bomb")
    {
      CHECK(model.GetButton(1, 0).GetButtonState() == fsweep::ButtonState::None);
      CHECK(model.GetButton(1, 0).GetHasBomb() == true);
    }

    THEN("The Button at (2, 0) is Down state with a bomb")
    {
      CHECK(model.GetButton(2, 0).GetButtonState() == fsweep::ButtonState::Down);
      CHECK(model.GetButton(2, 0).GetHasBomb() == true);
    }

    THEN("The Button at (3, 0) is Flagged state with no bomb")
    {
      CHECK(model.GetButton(3, 0).GetButtonState() == fsweep::ButtonState::Flagged);
      CHECK(model.GetButton(3, 0).GetHasBomb() == false);
    }

    THEN("The Button at (4, 0) is Flagged state with a bomb")
    {
      CHECK(model.GetButton(4, 0).GetButtonState() == fsweep::ButtonState::Flagged);
      CHECK(model.GetButton(4, 0).GetHasBomb() == true);
    }

    THEN("The Button at (5, 0) is Questioned state with no bomb")
    {
      CHECK(model.GetButton(5, 0).GetButtonState() == fsweep::ButtonState::Questioned);
      CHECK(model.GetButton(5, 0).GetHasBomb() == false);
    }

    THEN("The Button at (6, 0) is Questioned state with a bomb")
    {
      CHECK(model.GetButton(6, 0).GetButtonState() == fsweep::ButtonState::Questioned);
      CHECK(model.GetButton(6, 0).GetHasBomb() == true);
    }

    THEN("The Button at (7, 0) is None state with no bomb")
    {
      CHECK(model.GetButton(7, 0).GetButtonState() == fsweep::ButtonState::None);
      CHECK(model.GetButton(7, 0).GetHasBomb() == false);
    }

    THEN("The flag count is 2") { CHECK(model.GetFlagCount() == 2); }
  }
}

SCENARIO("A new game is started in a Model without a configuration change")
{
  GIVEN("A Model in dead game state with enabled questions")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), true,
                        fsweep::GameState::Dead, 30,
                        "dbxfcqr."
                        "........"
                        "........"
                        "........"
                        "........"
                        "........"
                        "........"
                        "........");

    WHEN("A new game is started without a configuration change")
    {
      model.NewGame();

      THEN("The GameState is None") { CHECK(model.GetGameState() == fsweep::GameState::None); }

      THEN("Questions are still enabled") { CHECK(model.GetQuestionsEnabled() == true); }

      THEN("The GameConfiguration has beginner difficulty")
      {
        CHECK(model.GetGameConfiguration().GetGameDifficulty() == fsweep::GameDifficulty::Beginner);
        CHECK(model.GetGameConfiguration().GetButtonsWide() ==
              fsweep::GameConfiguration::BEGINNER_BUTTONS_WIDE);
        CHECK(model.GetGameConfiguration().GetButtonsTall() ==
              fsweep::GameConfiguration::BEGINNER_BUTTONS_TALL);
        CHECK(model.GetGameConfiguration().GetBombCount() ==
              fsweep::GameConfiguration::BEGINNER_BOMB_COUNT);
      }

      THEN("The timer has 0") { CHECK(model.GetGameTime() == 0); }

      THEN("The Button objects are all default constructed")
      {
        CHECK(model.GetButton(0, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(0, 0).GetHasBomb() == false);
        CHECK(model.GetButton(1, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(1, 0).GetHasBomb() == false);
        CHECK(model.GetButton(2, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(2, 0).GetHasBomb() == false);
        CHECK(model.GetButton(3, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(3, 0).GetHasBomb() == false);
        CHECK(model.GetButton(4, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(4, 0).GetHasBomb() == false);
        CHECK(model.GetButton(5, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(5, 0).GetHasBomb() == false);
        CHECK(model.GetButton(6, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(6, 0).GetHasBomb() == false);
        CHECK(model.GetButton(7, 0).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(7, 0).GetHasBomb() == false);
      }
    }
  }
}

SCENARIO("A new game is started in a Model with a configuration change")
{
  GIVEN("A Model with beginner difficulty")
  {
    fsweep::Model model;

    WHEN("A new game is started with intermediate difficulty")
    {
      model.NewGame(fsweep::GameConfiguration(fsweep::GameDifficulty::Intermediate));

      THEN("The new game is intermediate difficulty")
      {
        CHECK(model.GetGameConfiguration().GetGameDifficulty() ==
              fsweep::GameDifficulty::Intermediate);
      }

      THEN(
          "The amount of Button objects in the buttons vector the amount for intermediate "
          "difficulty")
      {
        CHECK(model.GetButtons().size() ==
              fsweep::GameConfiguration::INTERMEDIATE_BUTTONS_WIDE *
                  fsweep::GameConfiguration::INTERMEDIATE_BUTTONS_TALL);
      }

      THEN("The GameState is None") { CHECK(model.GetGameState() == fsweep::GameState::None); }
    }
  }

  GIVEN("A Model constructed with one button pressed in playing state")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), false,
                        fsweep::GameState::Playing, 55,
                        "db......"
                        "bb......"
                        ".b....b."
                        "........"
                        "...b...."
                        "........"
                        ".b....b."
                        "........");

    WHEN("A new game is started with intermediate difficulty")
    {
      model.NewGame(fsweep::GameConfiguration(fsweep::GameDifficulty::Intermediate));

      THEN("The new game is intermediate difficulty")
      {
        CHECK(model.GetGameConfiguration().GetGameDifficulty() ==
              fsweep::GameDifficulty::Intermediate);
      }

      THEN(
          "The amount of Button objects in the buttons vector the amount for intermediate "
          "difficulty")
      {
        CHECK(model.GetButtons().size() ==
              fsweep::GameConfiguration::INTERMEDIATE_BUTTONS_WIDE *
                  fsweep::GameConfiguration::INTERMEDIATE_BUTTONS_TALL);
      }

      THEN("The GameState is None") { CHECK(model.GetGameState() == fsweep::GameState::None); }
    }
  }
}

SCENARIO("A Button of a Model is clicked")
{
  GIVEN("A default constructed Model")
  {
    fsweep::Model model;

    WHEN("A Button is clicked")
    {
      model.ClickButton(6, 6);

      THEN("The Button is Down")
      {
        CHECK(model.GetButton(6, 6).GetButtonState() == fsweep::ButtonState::Down);
      }

      THEN("The GameState is Playing")
      {
        CHECK(model.GetGameState() == fsweep::GameState::Playing);
      }

      THEN("The Model has the correct amount of bombs")
      {
        int bomb_count = 0;
        for (auto& button : model.GetButtons())
        {
          if (button.GetHasBomb() == true)
          {
            bomb_count++;
          }
        }
        CHECK(bomb_count == fsweep::GameConfiguration::BEGINNER_BOMB_COUNT);
      }

      THEN("The clicked button has no bombs")
      {
        CHECK(model.GetButton(6, 6).GetHasBomb() == false);
      }
    }
  }

  GIVEN("A Model is in playing state")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), false,
                        fsweep::GameState::Playing, 0,
                        "b......."
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("A bombed Button is clicked")
    {
      model.ClickButton(0, 0);

      THEN("The GameState is Dead") { CHECK(model.GetGameState() == fsweep::GameState::Dead); }
    }
  }

  GIVEN("A Model in dead state")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), false,
                        fsweep::GameState::Dead, 0,
                        "x......."
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("A Button with None state is clicked")
    {
      model.ClickButton(5, 5);

      THEN("The Button is not down")
      {
        CHECK(model.GetButton(5, 5).GetButtonState() == fsweep::ButtonState::None);
      }
    }
  }

  GIVEN("A Model in cool state")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), false,
                        fsweep::GameState::Cool, 0,
                        "bddddddd"
                        "cbdddddd"
                        "ddddddbd"
                        "dddddddd"
                        "dddddddd"
                        "dddddddd"
                        "dbddddbb"
                        "ddddddbd");

    WHEN("A bombed Button is clicked")
    {
      model.ClickButton(0, 0);

      THEN("The Button is not down")
      {
        CHECK(model.GetButton(0, 0).GetButtonState() == fsweep::ButtonState::None);
      }
    }

    WHEN("A flagged Button is clicked")
    {
      model.ClickButton(0, 1);

      THEN("The Button is still flagged and not down")
      {
        CHECK(model.GetButton(0, 1).GetButtonState() == fsweep::ButtonState::Flagged);
      }
    }
  }

  GIVEN("A Model in playing state)")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), false,
                        fsweep::GameState::Playing, 0,
                        "bf......"
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("A flagged Button is clicked")
    {
      model.ClickButton(1, 0);

      THEN("The Button is not down and still flagged")
      {
        CHECK(model.GetButton(1, 0).GetButtonState() == fsweep::ButtonState::Flagged);
      }
    }
  }

  GIVEN("A Model in playing state")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), true,
                        fsweep::GameState::Playing, 0,
                        "bq......"
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("A questioned Button is clicked")
    {
      model.ClickButton(1, 0);

      THEN("The GameState is still playing")
      {
        CHECK(model.GetGameState() == fsweep::GameState::Playing);
      }

      THEN("The questioned Button is down")
      {
        CHECK(model.GetButton(1, 0).GetButtonState() == fsweep::ButtonState::Down);
      }
    }

    WHEN("A Button in the middle of an bombless are is clicked")
    {
      model.ClickButton(4, 4);

      THEN("The bombless area was floodfill pressed")
      {
        CHECK(model.GetButton(0, 4).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(4, 3).GetButtonState() == fsweep::ButtonState::Down);
      }
    }
  }
}

SCENARIO("A Button of a Model is alt clicked")
{
  GIVEN("A default constructed Model")
  {
    fsweep::Model model;

    WHEN("The Button at (0, 0) is alt clicked")
    {
      model.AltClickButton(0, 0);

      THEN("The state of the Button at (0, 0) is None")
      {
        CHECK(model.GetButton(0, 0).GetButtonState() == fsweep::ButtonState::None);
      }

      THEN("The Button at (0, 0) has no bomb")
      {
        CHECK(model.GetButton(0, 0).GetHasBomb() == false);
      }
    }
  }

  GIVEN("A Model in playing state with questions enabled")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), true,
                        fsweep::GameState::Playing, 0,
                        "bfq....."
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("A down Button is alt clicked")
    {
      model.AltClickButton(7, 7);

      THEN("The Button is still down")
      {
        CHECK(model.GetButton(7, 7).GetButtonState() == fsweep::ButtonState::Down);
      }
    }

    WHEN("A questioned Button is alt clicked")
    {
      model.AltClickButton(2, 0);

      THEN("The Button is now has None state")
      {
        CHECK(model.GetButton(2, 0).GetButtonState() == fsweep::ButtonState::None);
      }
    }

    WHEN("A flagged Button is alt clicked")
    {
      model.AltClickButton(1, 0);

      THEN("The Button now has Questioned state")
      {
        CHECK(model.GetButton(1, 0).GetButtonState() == fsweep::ButtonState::Questioned);
      }

      THEN("The flag count is decremented") { CHECK(model.GetFlagCount() == 0); }
    }

    WHEN("A Button with None state is alt clicked")
    {
      model.AltClickButton(3, 0);

      THEN("The Button is now flagged")
      {
        CHECK(model.GetButton(3, 0).GetButtonState() == fsweep::ButtonState::Flagged);
      }

      THEN("The flag count is incremented") { CHECK(model.GetFlagCount() == 2); }
    }
  }

  GIVEN("A Model in playing state with questions disabled")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), false,
                        fsweep::GameState::Playing, 0,
                        "bf......"
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("A down Button is alt clicked")
    {
      model.AltClickButton(7, 7);

      THEN("The Button is still down")
      {
        CHECK(model.GetButton(7, 7).GetButtonState() == fsweep::ButtonState::Down);
      }
    }

    WHEN("A flagged Button is alt clicked")
    {
      model.AltClickButton(1, 0);

      THEN("The Button now has None state")
      {
        CHECK(model.GetButton(1, 0).GetButtonState() == fsweep::ButtonState::None);
      }

      THEN("The flag count is decremented") { CHECK(model.GetFlagCount() == 0); }
    }

    WHEN("A Button with None state is alt clicked")
    {
      model.AltClickButton(3, 0);

      THEN("The Button is now flagged")
      {
        CHECK(model.GetButton(3, 0).GetButtonState() == fsweep::ButtonState::Flagged);
      }

      THEN("The flag count is incremented") { CHECK(model.GetFlagCount() == 2); }
    }
  }
}

SCENARIO("A Button of a Model is area clicked")
{
  GIVEN("A Model with GameState::None")
  {
    fsweep::Model model;

    WHEN("A Button is area clicked")
    {
      model.AreaClickButton(5, 5);

      THEN("The GameState is None") { CHECK(model.GetGameState() == fsweep::GameState::None); }

      THEN("All Button objects in the area still have None state")
      {
        CHECK(model.GetButton(4, 4).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(5, 4).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(6, 4).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(4, 5).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(5, 5).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(6, 5).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(4, 6).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(5, 6).GetButtonState() == fsweep::ButtonState::None);
        CHECK(model.GetButton(6, 6).GetButtonState() == fsweep::ButtonState::None);
      }
    }
  }

  GIVEN("A Model in playing state")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), true,
                        fsweep::GameState::Playing, 0,
                        "bq......"
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("A Button is area clicked and no bombs are hit")
    {
      model.AreaClickButton(4, 4);

      THEN("The GameState is still playing")
      {
        CHECK(model.GetGameState() == fsweep::GameState::Playing);
      }

      THEN("All the area clicked Button objects are down")
      {
        CHECK(model.GetButton(3, 3).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(4, 3).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(5, 3).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(3, 4).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(4, 4).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(5, 4).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(3, 5).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(4, 5).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(5, 5).GetButtonState() == fsweep::ButtonState::Down);
      }

      THEN("Button objects outside of the area that are bombless get flood fill clicked")
      {
        CHECK(model.GetButton(5, 0).GetButtonState() == fsweep::ButtonState::Down);
      }
    }

    WHEN("A Button is area clicked and a bomb is hit")
    {
      model.AreaClickButton(5, 2);

      THEN("The GameState is Dead") { CHECK(model.GetGameState() == fsweep::GameState::Dead); }

      THEN("All the area clicked Button objects are down")
      {
        CHECK(model.GetButton(4, 1).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(5, 1).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(6, 1).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(4, 2).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(5, 2).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(6, 2).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(4, 3).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(5, 3).GetButtonState() == fsweep::ButtonState::Down);
        CHECK(model.GetButton(6, 3).GetButtonState() == fsweep::ButtonState::Down);
      }

      THEN("Button objects outside of the area that are bombless get flood fill clicked")
      {
        CHECK(model.GetButton(5, 0).GetButtonState() == fsweep::ButtonState::Down);
      }
    }
  }
}

SCENARIO("The game time of a Model is updated")
{
  GIVEN("A Model with GameState::None")
  {
    fsweep::Model model;

    WHEN("The timer is updated")
    {
      model.UpdateTime(14);

      THEN("The timer value is still 0") { CHECK(model.GetGameTime() == 0); }
    }
  }

  GIVEN("A Model with GameState::Playing")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), true,
                        fsweep::GameState::Playing, 0,
                        "bq......"
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("The timer is updated")
    {
      model.UpdateTime(14);

      THEN("The timer value is correct") { CHECK(model.GetGameTime() == 14); }

      WHEN("The timer is updated again")
      {
        model.UpdateTime(16);

        THEN("The timer value is correct") { CHECK(model.GetGameTime() == 30); }
      }
    }
  }
}

SCENARIO("Questions are enabled or disabled throughout a Model")
{
  GIVEN("A Model with GameState::None with questions disabled")
  {
    fsweep::Model model;

    WHEN("Questions are enabled")
    {
      model.SetQuestionsEnabled(true);

      THEN("Questions are enabled") { CHECK(model.GetQuestionsEnabled() == true); }
    }
  }

  GIVEN("A Model with GameState::Playing with questions enabled")
  {
    fsweep::Model model(fsweep::GameConfiguration(fsweep::GameDifficulty::Beginner), true,
                        fsweep::GameState::Playing, 0,
                        "bfq....."
                        "bb......"
                        "......b."
                        "........"
                        "........"
                        "........"
                        ".b....bb"
                        "......bd");

    WHEN("Questions are disabled")
    {
      model.SetQuestionsEnabled(false);

      THEN("All questioned buttons now have ButtonState::None")
      {
        CHECK(model.GetButton(2, 0).GetButtonState() == fsweep::ButtonState::None);
      }
    }
  }
}