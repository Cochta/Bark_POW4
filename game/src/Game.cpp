#include "Game.h"

std::string Game::GetName() noexcept { return "Bouncing Ground"; }

std::string Game::GetDescription() noexcept {
  return "CONTROLS: Left click to create a circle, right click to create a "
         "rectangle.\n\nFloor has 1 x bounciness, Circles and rectangles have "
         "0 x bounciness";
}

void Game::OnCollisionEnter(ColliderRef col1, ColliderRef col2) noexcept {}

void Game::OnCollisionExit(ColliderRef col1, ColliderRef col2) noexcept {}

void Game::SceneSetUp() noexcept {
  _world.SetContactListener(this);

  _font.loadFromFile("SFML/ressources/LiberationSans.ttf");
  _rectGameState.setOrigin(_rectGameState.getSize().x / 2,
                           _rectGameState.getSize().y / 2);
  _rectGameState.setPosition(
      Metrics::Width / 2,
      Metrics::Height - _rectGameState.getGlobalBounds().height / 2);
  _rectGameState.setOutlineColor(sf::Color::Blue);
  _rectGameState.setOutlineThickness(5);
  _rectGameState.setFillColor({80, 80, 80});
  _textGameState.setFont(_font);
  _textGameState.setColor(sf::Color::Yellow);

  _textP1.setFont(_font);
  _textP1.setColor(sf::Color::Black);
  _textP2.setFont(_font);
  _textP2.setColor(sf::Color::Black);

  _rectP1.setOrigin(_rectP1.getSize().x / 2, _rectP1.getSize().y / 2);
  _rectP1.setPosition(_rectP1.getSize().x / 2, _rectP1.getSize().y / 2);
  _rectP1.setFillColor(sf::Color::Yellow);

  _rectP2.setOrigin(_rectP2.getSize().x / 2, _rectP2.getSize().y / 2);
  _rectP2.setPosition(Metrics::Width - _rectP2.getSize().x / 2,
                      _rectP2.getSize().y / 2);
  _rectP2.setFillColor(sf::Color::Red);

  GraphicsData gd;
  gd.Color = sf::Color::Blue;
  // Create static rectangle
  for (size_t i = 0; i < 7; i++) {
    const auto groundRef = _world.CreateBody();
    _bodyRefs.push_back(groundRef);
    auto& groundBody = _world.GetBody(groundRef);
    groundBody.Type = BodyType::STATIC;
    groundBody.Mass = 1;

    groundBody.Position = {
        Metrics::Width / 2.f,
        Metrics::Height - Metrics::Height / 5.f - i * Metrics::Width / 15.f};

    const auto groundColRef = _world.CreateCollider(groundRef);
    _colRefs.push_back(groundColRef);
    auto& groundCol = _world.GetCollider(groundColRef);
    if (i != 0) {
      groundCol.IsTrigger = true;
    }
    auto rect = Math::RectangleF({-Metrics::Width / 3.f, 0.f},
                                 {Metrics::Width / 3.f, Metrics::Width / 60.f});
    /*rect.*/
    groundCol.Shape = rect;

    groundCol.BodyPosition = groundBody.Position;
    groundCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);
  }

  for (int i = -4; i < 5; ++i) {
    // Create static rectangle
    const auto wallRef = _world.CreateBody();
    _bodyRefs.push_back(wallRef);
    auto& wallBody = _world.GetBody(wallRef);
    wallBody.Type = BodyType::STATIC;
    wallBody.Mass = 1;

    wallBody.Position = {Metrics::Width / 2.f + Metrics::Width / 12.f * i,
                         Metrics::Height / 2.f};

    const auto wallColRef = _world.CreateCollider(wallRef);
    _colRefs.push_back(wallColRef);
    auto& wallCol = _world.GetCollider(wallColRef);
    wallCol.Shape =
        Math::RectangleF({-Metrics::Width / 120.f, -Metrics::Width / 4.f},
                         {Metrics::Width / 120.f, Metrics::Width / 4.f});
    wallCol.BodyPosition = wallBody.Position;
    wallCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);

    // Create static circles
    const auto roofRef = _world.CreateBody();
    _bodyRefs.push_back(roofRef);
    auto& roofBody = _world.GetBody(roofRef);
    roofBody.Type = BodyType::STATIC;
    roofBody.Mass = 1;

    roofBody.Position = {Metrics::Width / 2.f + Metrics::Width / 12.f * i,
                         Metrics::Height / 8.f};

    const auto roofColRef = _world.CreateCollider(roofRef);
    _colRefs.push_back(roofColRef);
    auto& roofCol = _world.GetCollider(roofColRef);
    roofCol.Shape = Math::CircleF(Math::Vec2F::Zero(), Metrics::Width / 120.f);
    roofCol.BodyPosition = roofBody.Position;
    roofCol.Restitution = 0.f;

    AllGraphicsData.emplace_back(gd);
  }
}
void Game::SceneUpdate() noexcept {
  if (_hasGameEnded && !_isLobbyDestroyed) {
    _isLobbyDestroyed = true;
    auto packet = new GameFinishedPacket();
    if (_hasP1Won) {
      packet->HasP1Won = true;
    } else if (_hasP2Won) {
      packet->HasP1Won = false;
    }
    _client->SendPacket(packet);
  }
  if (!_hasGameEnded) {
    _draw = true;
    for (int i = -4; i < 4; ++i) {
      _rectPutBall.setOrigin(_rectPutBall.getSize().x / 2,
                             _rectPutBall.getSize().y / 2);
      _rectPutBall.setPosition(Metrics::Width / 2.f + Metrics::Width / 25.f +
                                   Metrics::Width / 12.f * i,
                               Metrics::Width / 60.f);

      if (_gb.board[{i + 4, 6}] == GameColor::None) {
        if (_rectPutBall.getGlobalBounds().contains(_mousePos.X, _mousePos.Y)) {
          _rectPutBall.setFillColor({120, 120, 120});
          if (_mouseLeftReleased && IsPlayerTurn) {
            CreateBall(_mousePos, i + 4);
            auto p = new HasPlayedPacket();
            p->IsFirstTurn = false;
            p->X = _mousePos.X;
            p->Y = _mousePos.Y;
            p->index = i + 4;
            _client->SendPacket(p);
            IsPlayerTurn = false;
          }
        } else {
          _rectPutBall.setFillColor({60, 60, 60});
        }
        _window->draw(_rectPutBall);
        _draw = false;
      }
    }
  }

  DrawGameState();

  for (std::size_t i = 0; i < _colRefs.size(); ++i) {
    const auto& col = _world.GetCollider(_colRefs[i]);

    const auto& shape = _world.GetCollider(_colRefs[i]).Shape;

    switch (shape.index()) {
      case static_cast<int>(Math::ShapeType::Circle):
        _world.GetBody(col.BR).ApplyForce({0, SPEED});
        AllGraphicsData[i].Shape =
            std::get<Math::CircleF>(shape) + col.BodyPosition;
        break;
      case static_cast<int>(Math::ShapeType::Rectangle):

        AllGraphicsData[i].Shape =
            std::get<Math::RectangleF>(shape) +
            Math::Vec2F(
                col.BodyPosition.X,
                col.BodyPosition.Y - (i < 7 ? Metrics::Width / 120.f : 0));
        break;
      default:
        break;
    }
  }
}

void Game::SceneTearDown() noexcept {
  _gb = GameBoard();

  _draw = false;
  _hasGameEnded = false;
  _isLobbyDestroyed = false;
  _hasP1Won = false;
  _hasP2Won = false;
  _isHoverButton = false;

  IsPlayer1 = false;
  IsPlayerTurn = false;
}

bool Game::CheckVictory(int x, int y) noexcept {
  // check for ROW
  GameColor color = _gb.board[{0, y}];
  int nbSame = 1;
  for (int i = 1; i < 8; ++i) {
    GameColor actualColor = _gb.board[{i, y}];

    if (actualColor != GameColor::None && actualColor == color) {
      nbSame++;
      if (nbSame >= 4) {
        return true;
      }
    } else if (actualColor != GameColor::None) {
      color = actualColor;
      nbSame = 1;
    } else {
      nbSame = 0;
    }
  }

  // check for COLUMN
  color = _gb.board[{x, 0}];
  nbSame = 1;
  for (int i = 1; i < 7; ++i) {
    GameColor actualColor = _gb.board[{x, i}];

    if (actualColor != GameColor::None && actualColor == color) {
      nbSame++;
      if (nbSame >= 4) {
        return true;
      }
    } else if (actualColor != GameColor::None) {
      color = actualColor;
      nbSame = 1;
    } else {
      nbSame = 0;
    }
  }

  // diagonal from down left to up right
  color = _gb.board[{x, y}];
  nbSame = 1;
  int row = x, col = y;
  while (row > 0 && col > 0) {
    row--;
    col--;
    GameColor actualColor = _gb.board[{row, col}];
    if (color == actualColor) {
      nbSame++;

    } else {
      break;
    }
  }
  row = x;
  col = y;
  while (row < 7 && col < 6) {
    row++;
    col++;
    GameColor actualColor = _gb.board[{row, col}];
    if (color == actualColor) {
      nbSame++;
    } else {
      break;
    }
  }
  if (nbSame >= 4) {
    return true;
  }

  // diagonal from up left to down right
  color = _gb.board[{x, y}];
  nbSame = 1;
  row = x, col = y;
  while (row > 0 && col < 6) {
    row--;
    col++;
    GameColor actualColor = _gb.board[{row, col}];
    if (color == actualColor) {
      nbSame++;

    } else {
      break;
    }
  }
  row = x;
  col = y;
  while (row < 7 && col > 0) {
    row++;
    col--;
    GameColor actualColor = _gb.board[{row, col}];
    if (color == actualColor) {
      nbSame++;
    } else {
      break;
    }
  }
  if (nbSame >= 4) {
    return true;
  }
  return false;
}

void Game::DrawGameState() {
  if (_draw) {
    _textGameState.setString("Draw !!!");
    _hasGameEnded = true;
  } else if (_hasP1Won) {
    _textGameState.setString("!! YELLOW WINS !!");
  } else if (_hasP2Won) {
    _textGameState.setString("!! RED WINS !!");
  } else if (!_hasGameEnded) {
    if (IsPlayerTurn) {
      _textGameState.setString("Your turn, press on grey squares to play");
    } else {
      _textGameState.setString("Waiting for other player ...");
    }
  }

  // Calculate text bounds
  _textBounds = _textGameState.getLocalBounds();
  // Set origin to the center of the text bounds

  _textGameState.setOrigin(_textBounds.left + _textBounds.width / 2.0f,
                           _textBounds.top + _textBounds.height / 2.0f);

  _textGameState.setPosition(_rectGameState.getPosition());

  _window->draw(_rectGameState);
  _window->draw(_textGameState);
}

void Game::CreateBall(Math::Vec2F position, int index) noexcept {
  for (int i = 0; i < 8; ++i) {
    if (_gb.board[{index, i}] == GameColor::None) {
      _gb.board[{index, i}] = IsPlayer1 ? GameColor::Yellow : GameColor::Red;
      if (CheckVictory(index, i)) {
        _hasGameEnded = true;
        if (IsPlayer1) {
          _hasP1Won = true;
        } else if (!IsPlayer1) {
          _hasP2Won = true;
        }
      }

      break;
    }
  }

  const auto circleBodyRef = _world.CreateBody();
  _bodyRefs.push_back(circleBodyRef);
  auto& circleBody = _world.GetBody(circleBodyRef);

  circleBody.Mass = 1;

  circleBody.Position = position;

  const auto circleColRef = _world.CreateCollider(circleBodyRef);
  _colRefs.push_back(circleColRef);
  auto& circleCol = _world.GetCollider(circleColRef);
  circleCol.Shape = Math::Circle(Math::Vec2F::Zero(), Metrics::Width / 30.f);
  circleCol.BodyPosition = circleBody.Position;
  circleCol.Restitution = 0.f;

  GraphicsData gd;
  if (IsPlayer1) {
    gd.Color = {255, 255, 0};
  } else {
    gd.Color = {255, 0, 0};
  }

  AllGraphicsData.emplace_back(gd);
}

bool Game::UpdateQuitButton() {
  // Add a quit button
  sf::RectangleShape quitButton(
      sf::Vector2f(Metrics::Width / 6.f, Metrics::Height / 6.f));

  // Text for the quit button
  std::string str = "Surrender";
  if (_hasGameEnded) {
    str = "leave";
  }
  sf::Text quitButtonText(str, _font);
  quitButtonText.setFillColor(sf::Color::White);
  sf::FloatRect quitTextBounds = quitButtonText.getLocalBounds();
  quitButtonText.setOrigin(quitTextBounds.left + quitTextBounds.width / 2.0f,
                           quitTextBounds.top + quitTextBounds.height / 2.0f);

  quitButton.setPosition(0, Metrics::Height - Metrics::Height / 6.f);

  quitButtonText.setPosition(
      quitButton.getPosition().x + quitButton.getSize().x / 2,
      quitButton.getPosition().y + quitButton.getSize().y / 2);

  // Check if mouse is hovering over the quit button
  if (quitButton.getGlobalBounds().contains(
          sf::Vector2f(_mousePos.X, _mousePos.Y))) {
    quitButton.setFillColor(sf::Color::Magenta);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      _client->SendPacket(new SurrenderPacket());
      return true;
    }
  } else {
    quitButton.setFillColor(sf::Color::Red);
  }
  _window->draw(quitButton);
  _window->draw(quitButtonText);
  return false;
}

void Game::OtherPlayerHasSurrendered() noexcept {  // todo: ptn ca marche pas
  _hasGameEnded = true;
  if (IsPlayer1) {
    _hasP1Won = true;
  } else {
    _hasP2Won = true;
  }
}

void Game::DrawPlayerData(std::string_view name, int elo, PlayerData p1,
                          PlayerData p2) {
  if (_hasP1Won) {
    _textP1.setString(p1.name + "\n" + std::to_string(p1.elo + 30));
    _textP2.setString(p2.name + "\n" + std::to_string(p2.elo - 30));
  } else if (_hasP2Won) {
    _textP1.setString(p1.name + "\n" + std::to_string(p1.elo - 30));
    _textP2.setString(p2.name + "\n" + std::to_string(p2.elo + 30));
  } else {
    _textP1.setString(p1.name + "\n" + std::to_string(p1.elo));
    _textP2.setString(p2.name + "\n" + std::to_string(p2.elo));
  }

  // Calculate text bounds
  _textBounds = _textP1.getLocalBounds();
  // Set origin to the center of the text bounds

  _textP1.setOrigin(_textBounds.left + _textBounds.width / 2.0f,
                    _textBounds.top + _textBounds.height / 2.0f);

  _textP1.setPosition(_rectP1.getPosition());

  // Calculate text bounds
  _textBounds = _textP2.getLocalBounds();
  // Set origin to the center of the text bounds

  _textP2.setOrigin(_textBounds.left + _textBounds.width / 2.0f,
                    _textBounds.top + _textBounds.height / 2.0f);

  _textP2.setPosition(_rectP2.getPosition());

  _window->draw(_rectP1);
  _window->draw(_rectP2);
  _window->draw(_textP1);
  _window->draw(_textP2);
}
