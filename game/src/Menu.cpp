#include "Menu.h"

std::string Menu::GetName() noexcept { return "Menu"; }

std::string Menu::GetDescription() noexcept { return ""; }

void Menu::StartConnection() noexcept {
  if (_isHoverButton && !_isWaitingForConnection) {
    _client->SendPacket(new ConnectPacket());
    _isWaitingForConnection = true;
    _isHoverButton = false;
  }
}

void Menu::SceneSetUp() noexcept {
  _rectStart.setOrigin(_rectStart.getSize().x / 2, _rectStart.getSize().y / 2);
  _rectStart.setPosition(Metrics::Width / 2, Metrics::Height / 2);
  _rectStart.setOutlineColor(sf::Color::Red);
  _rectStart.setOutlineThickness(5);

  _font.loadFromFile("SFML/ressources/LiberationSans.ttf");
}
void Menu::SceneUpdate() noexcept {
  if (_rectStart.getGlobalBounds().contains(
          sf::Vector2f(_mousePos.X, _mousePos.Y))) {
    _rectStart.setFillColor(sf::Color::Green);
    _isHoverButton = true;
  } else {
    _isHoverButton = false;
    _rectStart.setFillColor(sf::Color::Blue);
  }

  _textStart.setFont(_font);
  if (_isWaitingForConnection) {
    _textStart.setString("Waiting for other player");
  } else {
    _textStart.setString("Start Game");
  }

  // Calculate text bounds
  sf::FloatRect textBounds = _textStart.getLocalBounds();
  // Set origin to the center of the text bounds
  _textStart.setOrigin(textBounds.left + textBounds.width / 2.0f,
                       textBounds.top + textBounds.height / 2.0f);

  _textStart.setPosition(Metrics::Width / 2, Metrics::Height / 2);
  _textStart.setColor(sf::Color::Yellow);

  _window->draw(_rectStart);
  _window->draw(_textStart);
}

void Menu::SceneTearDown() noexcept {}