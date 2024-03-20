#include "Menu.h"

std::string Menu::GetName() noexcept { return "Menu"; }

std::string Menu::GetDescription() noexcept { return ""; }

void Menu::StartConnection(bool isConnectedToServer) noexcept {
  _isConnectedToServer = isConnectedToServer;
  if (_isHoverButton && !_isWaitingForConnection) {
    _client->SendPacket(new ConnectPacket());
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
  if (_isWaitingForConnection && _isConnectedToServer) {
    _textStart.setString("Waiting for other player");
  } else if (_isWaitingForConnection && !_isConnectedToServer) {
    _textStart.setString(" Server is busy\nplease try again");
  } else {
    _textStart.setString("Start Game");
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