#include "Menu.h"

std::string Menu::GetName() noexcept { return "Menu"; }

std::string Menu::GetDescription() noexcept { return ""; }

void Menu::StartConnection(bool isConnectedToServer) noexcept {
  _isConnectedToServer = isConnectedToServer;
  if (_isHoverButton && !_isWaitingForConnection && _playerName.size() >= 4) {
    _client->SendPacket(new ConnectPacket(_playerName));
    _isWaitingForConnection = true;
    _isHoverButton = false;
  }
}

bool Menu::UpdateQuitButton() {
  // Add a quit button
  sf::RectangleShape quitButton(
      sf::Vector2f(Metrics::Width / 5.f, Metrics::Height / 6.f));

  // Text for the quit button
  sf::Text quitButtonText("Quit", _font);
  quitButtonText.setFillColor(sf::Color::White);
  sf::FloatRect quitTextBounds = quitButtonText.getLocalBounds();
  quitButtonText.setOrigin(quitTextBounds.left + quitTextBounds.width / 2.0f,
                           quitTextBounds.top + quitTextBounds.height / 2.0f);

  quitButton.setPosition(Metrics::Width / 2.f - Metrics::Width / 5.f / 2.f,
                         Metrics::Height - Metrics::Height / 6.f);

  quitButtonText.setPosition(
      quitButton.getPosition().x + quitButton.getSize().x / 2,
      quitButton.getPosition().y + quitButton.getSize().y / 2);

  // Check if mouse is hovering over the quit button
  if (quitButton.getGlobalBounds().contains(
          sf::Vector2f(_mousePos.X, _mousePos.Y))) {
    quitButton.setFillColor(sf::Color::Magenta);
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      _client->SendPacket(new QuitLobbyPacket());
      _window->close();
      exit(EXIT_SUCCESS);
    }
  } else {
    quitButton.setFillColor(sf::Color::Red);
  }
  _window->draw(quitButton);
  _window->draw(quitButtonText);

  return false;
}

void Menu::DrawPlayerData(std::string_view name, int elo, PlayerData p1,
                          PlayerData p2) {
  _playerName = name.data();
  _textPlayerName.setString("Enter your name, min 4 characters: \n\n[" +
                            _playerName + "]");
  // Calculate text bounds
  sf::FloatRect textBounds = _textPlayerName.getLocalBounds();
  // Set origin to the center of the text bounds
  _textPlayerName.setOrigin(textBounds.width * 0.5f, textBounds.height * 0.5f);

  _textPlayerName.setPosition(_rectPlayerName.getPosition());
  _textPlayerName.setColor(sf::Color::Yellow);

  _window->draw(_rectPlayerName);
  _window->draw(_textPlayerName);
}

void Menu::SceneSetUp() noexcept {
  _rectStart.setOrigin(_rectStart.getSize().x / 2, _rectStart.getSize().y / 2);
  _rectStart.setPosition(Metrics::Width / 2, Metrics::Height / 2);
  _rectStart.setOutlineColor(sf::Color::Red);
  _rectStart.setOutlineThickness(5);

  _rectPlayerName.setOrigin(_rectPlayerName.getSize().x / 2,
                            _rectPlayerName.getSize().y / 2);
  _rectPlayerName.setPosition(Metrics::Width / 2,
                              _rectPlayerName.getSize().y / 2);
  _rectPlayerName.setOutlineColor(sf::Color::Red);
  _rectPlayerName.setOutlineThickness(5);
  _rectPlayerName.setFillColor(sf::Color::Blue);

  _font.loadFromFile("SFML/ressources/LiberationSans.ttf");
  _textPlayerName.setFont(_font);
  _textPlayerName.setCharacterSize(20);
}
void Menu::SceneUpdate() noexcept {
  if (_rectStart.getGlobalBounds().contains(
          sf::Vector2f(_mousePos.X, _mousePos.Y)) &&
      _playerName.size() >= 4) {
    _rectStart.setFillColor(sf::Color::Green);
    _isHoverButton = true;
  } else {
    _isHoverButton = false;
    _rectStart.setFillColor(sf::Color::Blue);
  }

  _textStart.setFont(_font);
  if (_playerName.size() >= 4) {
    if (_isWaitingForConnection && _isConnectedToServer) {
      _textStart.setString("Waiting for other player");
    } else if (_isWaitingForConnection && !_isConnectedToServer) {
      _textStart.setString(" Server is busy\nplease try again");
    } else {
      _textStart.setString("Start Game");
    }
  } else {
    _textStart.setString("Enter valid name");
  }

  // Calculate text bounds
  sf::FloatRect textBounds = _textStart.getLocalBounds();
  // Set origin to the center of the text bounds
  _textStart.setOrigin(textBounds.left + textBounds.width / 2.0f,
                       textBounds.top + textBounds.height / 2.0f);

  _textStart.setPosition(Metrics::Width / 2.f, Metrics::Height / 2.f);
  _textStart.setColor(sf::Color::Yellow);

  _window->draw(_rectStart);
  _window->draw(_textStart);
}

void Menu::SceneTearDown() noexcept {
  _isConnectedToServer = false;
  _isWaitingForConnection = false;
}