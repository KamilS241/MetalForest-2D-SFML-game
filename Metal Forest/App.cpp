#include "App.h"

App::App()
{
	this->initWindow();
	this->gamePhase = 1;
}

void App::initWindow()
{
	vm.width = 1024;
	vm.height = 576;
	appWindowPtr = new sf::RenderWindow (vm, "Metal Forest");
	appWindowPtr->setFramerateLimit(60);
}

void App::mainMenuMethod()
{
	bool startGame = false;
	sf::Text metalForestText;
	sf::Text startGameText;
	sf::Text quitGameText;
	sf::Font f1;
	if (!f1.loadFromFile("Fonts/EvilSound-nRM8M.ttf"))
	{
		std::cout << "ERROR UNABLE TO LOAD FONTS" << std::endl;
	}
	sf::Font f2;
	if (!f2.loadFromFile("Fonts/AmaticSC-Regular.ttf"))
	{
		std::cout << "ERROR UNABLE TO LOAD FONTS" << std::endl;
	}
	metalForestText.setFont(f1);
	metalForestText.setCharacterSize(70);
	metalForestText.setFillColor(sf::Color::Red);
	metalForestText.setString("METAL FOREST");
	metalForestText.setPosition(vm.width / 2 - metalForestText.getGlobalBounds().width / 2, vm.height / 4 - metalForestText.getGlobalBounds().height / 2);
	startGameText.setFont(f2);
	startGameText.setCharacterSize(40);
	startGameText.setFillColor(sf::Color::White);
	startGameText.setString("START GAME");
	startGameText.setPosition(vm.width / 2 - startGameText.getGlobalBounds().width / 2, metalForestText.getPosition().y + 200.f);// vm.height / 2.5);
	quitGameText.setFont(f2);
	quitGameText.setCharacterSize(40);
	quitGameText.setFillColor(sf::Color::White);
	quitGameText.setString("QUIT GAME");
	quitGameText.setPosition(vm.width / 2 - quitGameText.getGlobalBounds().width / 2, startGameText.getPosition().y + 100.f);
	if (!backgroundTex.loadFromFile("Textures/MainMenuBackground.jpg"))
	{
		std::cout << "BACKGROUNDTEX ERROR" << std::endl;
	}
	background.setTexture(backgroundTex);
	while (!startGame)
	{
		mousePosWindow = sf::Mouse::getPosition(*appWindowPtr);
		mousePosView = appWindowPtr->mapPixelToCoords(mousePosWindow);
		startGameText.setFillColor(sf::Color::White);
		quitGameText.setFillColor(sf::Color::White);
		if (startGameText.getGlobalBounds().contains(mousePosView))
		{
			startGameText.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				appWindowPtr->clear();
				//soundtrack.stop();
				gamePhase = 2;
				soundtrackSet = false;
				return;
			}
		}
		else if (quitGameText.getGlobalBounds().contains(mousePosView))
		{
			quitGameText.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				gamePhase = 4;
				appWindowPtr->clear();
				soundtrackSet = false;
				return;
			}
		}
		appWindowPtr->clear();
		pollEvents();
		appWindowPtr->draw(background);
		appWindowPtr->draw(metalForestText);
		appWindowPtr->draw(startGameText);
		appWindowPtr->draw(quitGameText);
		appWindowPtr->display();
	}
}

void App::musicMethod()
{
	while (appWindowPtr->isOpen())
	{
		if (soundtrack.getStatus() != sf::Music::Status::Playing)
			soundtrackSet = false;
		if (soundtrackSet == false)
		{
			switch (gamePhase)
			{
			case 1:
			{
				if (!soundtrack.openFromFile("Soundtrack/SoundtrackMainMenu.wav"))
				{
					std::cout << "MAIN MENU MUSIC ERROR" << std::endl;
				}
				soundtrack.play();
				soundtrackSet = true;
				break;
			}
			case 2:
			{
				if (!soundtrack.openFromFile("Soundtrack/SoundtrackGame1.wav"))
				{
					std::cout << "GAME1 MUSIC ERROR" << std::endl;
				}
				soundtrack.play();
				soundtrackSet = true;
				break;
			}
			case 3:
			{
				if (!soundtrack.openFromFile("Soundtrack/SoundtrackEndGame.wav"))
				{
					std::cout << "ENDGAME MUSIC ERROR" << std::endl;
				}
				soundtrack.play();
				soundtrackSet = true;
				break;
			}
			case 4:
			{
				soundtrack.stop();
				return;
			}
			default: std::cout << "MUSIC ERROR" << std::endl;
			}
		}
	}
}

void App::gameMethod()
{
	std::srand(static_cast<unsigned>(time(NULL)));
	Game newGame(appWindowPtr, vm);
	while (newGame.getWindowStatus() && !newGame.getEndGame())
	{
		newGame.update();
		newGame.render();
		if (newGame.getEndGame())
		{
			gamePhase = 3;
			soundtrackSet = false;
			score = newGame.getScore();
		}
	}
}

void App::endGameMethod()
{
	sf::Sprite background;
	sf::Texture backgroundTex;
	if (!backgroundTex.loadFromFile("Textures/EndGameBackground.jpg"))
	{
		std::cout << "BACKGROUNDTEX ERROR" << std::endl;
	}
	background.setTexture(backgroundTex);
	sf::Text youAreNotTrveText;
	sf::Text tryAgainText;
	sf::Text yesText;
	sf::Text noText;
	sf::Text saveScoreText;
	sf::Text yourScoreText;
	sf::Text scoreText;
	sf::Text scoreBoardText;
	bool scoreSaved=false;
	sf::Font f1;
	if (!f1.loadFromFile("Fonts/AmaticSC-Regular.ttf"))
	{
		std::cout << "ERROR UNABLE TO LOAD FONTS" << std::endl;
	}
	youAreNotTrveText.setFont(f1);
	youAreNotTrveText.setCharacterSize(80);
	youAreNotTrveText.setFillColor(sf::Color::Red);
	youAreNotTrveText.setString("YOU ARE NOT TRVE");
	youAreNotTrveText.setPosition(vm.width / 2 - youAreNotTrveText.getGlobalBounds().width / 2, vm.height / 4 - youAreNotTrveText.getGlobalBounds().height / 2);
	tryAgainText.setFont(f1);
	tryAgainText.setCharacterSize(40);
	tryAgainText.setFillColor(sf::Color::White);
	tryAgainText.setString("Do you want to try again?");
	tryAgainText.setPosition(vm.width / 2 - tryAgainText.getGlobalBounds().width / 2, youAreNotTrveText.getPosition().y + 150.f);
	yesText.setFont(f1);
	yesText.setCharacterSize(40);
	yesText.setFillColor(sf::Color::White);
	yesText.setString("Yes");
	yesText.setPosition(vm.width / 2 - yesText.getGlobalBounds().width / 2 - 350, tryAgainText.getPosition().y + 150.f);
	noText.setFont(f1);
	noText.setCharacterSize(40);
	noText.setFillColor(sf::Color::White);
	noText.setString("No");
	noText.setPosition(vm.width / 2 - yesText.getGlobalBounds().width / 2 + 350, yesText.getPosition().y);
	yourScoreText.setFont(f1);
	yourScoreText.setCharacterSize(40);
	yourScoreText.setFillColor(sf::Color::White);
	yourScoreText.setString("Collected heads:");
	yourScoreText.setPosition(vm.width - vm.width / 6, youAreNotTrveText.getPosition().y + vm.height / 12);
	yourScoreText.setOrigin(yourScoreText.getGlobalBounds().width / 2, yourScoreText.getGlobalBounds().height / 2);
	scoreText.setFont(f1);
	scoreText.setCharacterSize(40);
	scoreText.setFillColor(sf::Color::Red);
	scoreText.setString(std::to_string(score));
	scoreText.setOrigin(scoreText.getGlobalBounds().width / 2, scoreText.getGlobalBounds().height / 2);
	scoreText.setPosition(yourScoreText.getPosition().x, yourScoreText.getPosition().y + vm.height / 10);
	saveScoreText.setFont(f1);
	saveScoreText.setCharacterSize(40);
	saveScoreText.setFillColor(sf::Color::White);
	saveScoreText.setString("Save");
	saveScoreText.setOrigin(saveScoreText.getGlobalBounds().width / 2, saveScoreText.getGlobalBounds().height / 2);
	saveScoreText.setPosition(yourScoreText.getPosition().x, scoreText.getPosition().y+vm.height/10);
	scoreBoardText.setFont(f1);
	scoreBoardText.setCharacterSize(40);
	scoreBoardText.setFillColor(sf::Color::White);
	scoreBoardText.setString("Scoreboard");
	scoreBoardText.setPosition(vm.width / 6, scoreText.getPosition().y);
	scoreBoardText.setOrigin(scoreBoardText.getGlobalBounds().width / 2, scoreBoardText.getGlobalBounds().height / 2);
	while (true)
	{
		mousePosWindow = sf::Mouse::getPosition(*appWindowPtr);
		mousePosView = appWindowPtr->mapPixelToCoords(mousePosWindow);
		yesText.setFillColor(sf::Color::White);
		noText.setFillColor(sf::Color::White);
		saveScoreText.setFillColor(sf::Color::White);
		scoreBoardText.setFillColor(sf::Color::White);
		if (yesText.getGlobalBounds().contains(mousePosView))
		{
			yesText.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				appWindowPtr->clear();
				gamePhase = 2;
				soundtrackSet = false;
				return;
			}
		}
		else if (noText.getGlobalBounds().contains(mousePosView))
		{
			noText.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				appWindowPtr->clear();
				gamePhase = 4;
				soundtrackSet = false;
				return;
			}
		}
		else if (saveScoreText.getGlobalBounds().contains(mousePosView) and scoreSaved==false)
		{
			saveScoreText.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				saveScore();
				saveScoreText.setString("Score saved");
				saveScoreText.setOrigin(saveScoreText.getGlobalBounds().width / 2, saveScoreText.getGlobalBounds().height / 2);
				saveScoreText.setPosition(scoreText.getPosition().x, scoreText.getPosition().y+ vm.height / 10);
				scoreSaved = true;
			}
		}
		else if (scoreBoardText.getGlobalBounds().contains(mousePosView))
		{
			scoreBoardText.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				this->showScoreBoard();
			}
		}
		pollEvents();
		appWindowPtr->clear();
		appWindowPtr->draw(background);
		appWindowPtr->draw(youAreNotTrveText);
		appWindowPtr->draw(tryAgainText);
		appWindowPtr->draw(yesText);
		appWindowPtr->draw(noText);
		appWindowPtr->draw(yourScoreText);
		appWindowPtr->draw(scoreText);
		appWindowPtr->draw(saveScoreText);
		appWindowPtr->draw(scoreBoardText);
		appWindowPtr->display();
	}
}

void App::saveScore()
{
	sf::Sprite background;
	sf::Texture backgroundTex;
	if (!backgroundTex.loadFromFile("Textures/SaveScoreBackground.png"))
	{
		std::cout << "BACKGROUNDTEX ERROR" << std::endl;
	}
	background.setTexture(backgroundTex);
	sf::Font f2;
	if (!f2.loadFromFile("Fonts/AmaticSC-Regular.ttf"))
	{
		std::cout << "ERROR UNABLE TO LOAD FONTS" << std::endl;
	}
	sf::Text enterYourNameText;
	sf::Text enteredName;
	sf::Text result;
	enterYourNameText.setFont(f2);
	enterYourNameText.setCharacterSize(60);
	enterYourNameText.setFillColor(sf::Color::White);
	enterYourNameText.setString("Enter Your name");
	enterYourNameText.setOrigin(enterYourNameText.getGlobalBounds().width / 2, enterYourNameText.getGlobalBounds().height / 2);
	enterYourNameText.setPosition(vm.width/4, vm.height/4);
	enteredName.setFont(f2);
	enteredName.setCharacterSize(45);
	enteredName.setFillColor(sf::Color::White);
	result.setFont(f2);
	result.setCharacterSize(35);
	result.setFillColor(sf::Color::Red);
	result.setOrigin(result.getGlobalBounds().width / 2, result.getGlobalBounds().height / 2);
	result.setPosition(enterYourNameText.getPosition().x, enterYourNameText.getPosition().y + 200);
	bool displayResult = false;
	std::string enteredText;
	bool nameReady=false;
	bool end = false;
	std::regex correctName ( "\\w{2,15}");
	while (!nameReady and appWindowPtr->isOpen())
	{
		sf::Event ev;
		while (appWindowPtr->pollEvent(ev))
		{
			if (ev.type == sf::Event::TextEntered)
			{
				if (ev.text.unicode < 128)
				{
					if (ev.text.unicode == 8 and !enteredText.empty())
					{
						enteredText.pop_back();
					}
					else if (ev.text.unicode != 8 and ev.text.unicode != 13 and ev.text.unicode!= 27)
					{
						enteredText += static_cast <char> (ev.text.unicode);
					}
					else if (ev.text.unicode == 13)
					{
						if (std::regex_match(enteredText, correctName))
						{
							result.setString("\n    Score saved");
							displayResult = true;
							uploadScoreToFile(enteredText);
							end = true;
						}
						else
						{
							displayResult = true;
							result.setString("Your name must contain: \n    At least 2 characters \n   And no more than 15");
						}
					}
				}
			}
		}
		enteredName.setString(enteredText);
		enteredName.setOrigin(enteredName.getGlobalBounds().width / 2, enteredName.getGlobalBounds().height / 2);
		enteredName.setPosition(enterYourNameText.getPosition().x, enterYourNameText.getPosition().y + 100);
		pollEvents();
		appWindowPtr->clear();
		appWindowPtr->draw(background);
		appWindowPtr->draw(enterYourNameText);
		appWindowPtr->draw(enteredName);
		if (displayResult)
			appWindowPtr->draw(result);
		appWindowPtr->display();
		if (end == true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(2));
			return;
		}
	}
}

void App::uploadScoreToFile(std::string name)
{
	std::filesystem::path scoresPath = std::filesystem::current_path() / "Scores";
	if (!std::filesystem::exists(scoresPath))
		std::filesystem::create_directory(scoresPath);
	std::filesystem::path nameDirPath = scoresPath / name;
	if (!std::filesystem::exists(nameDirPath))
		std::filesystem::create_directory(nameDirPath);
	std::time_t currentTime;
	std::tm localTimeInfo;
	char timeStr[100];
	if (std::time(&currentTime) != -1 && localtime_s(&localTimeInfo, &currentTime) == 0)
	{
		std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H-%M-%S", &localTimeInfo);
		std::string time = timeStr;
		std::string txt(".txt");
		std::filesystem::path newScore = nameDirPath / (time + txt);
		std::ofstream scoreStream(newScore);
		if (scoreStream.is_open())
		{
			scoreStream << "Score:\n" << score;
		}
		scoreStream.close();
	}
	else
		std::cout << "TIME OF SCORE ERROR" << std::endl;
}

void App::pollEvents()
{
	sf::Event ev;
	while (appWindowPtr->pollEvent(ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:
		{
			appWindowPtr->close();
			gamePhase = 4;
			break;
		}
		case sf::Event::KeyPressed:
		{
			if (ev.key.code == sf::Keyboard::Escape)
				appWindowPtr->close();
			gamePhase = 4;
			break;
		}
		}
	}
}

void App::showScoreBoard()
{
	sf::Sprite background;
	sf::Texture backgroundTex;
	if (!backgroundTex.loadFromFile("Textures/SaveScoreBackground.png"))
	{
		std::cout << "BACKGROUNDTEX ERROR" << std::endl;
	}
	background.setTexture(backgroundTex);
	sf::Font f2;
	if (!f2.loadFromFile("Fonts/AmaticSC-Regular.ttf"))
	{
		std::cout << "ERROR UNABLE TO LOAD FONTS" << std::endl;
	}
	sf::Text bestScoresText;
	sf::Text bestNames;
	sf::Text backText;
	bestScoresText.setFont(f2);
	bestScoresText.setCharacterSize(60);
	bestScoresText.setFillColor(sf::Color::White);
	bestScoresText.setString("Best scores:");
	bestScoresText.setPosition(vm.width / 7, vm.height / 7);
	bestNames.setFont(f2);
	bestNames.setCharacterSize(45);
	bestNames.setFillColor(sf::Color::White);
	bestNames.setOrigin(bestNames.getGlobalBounds().width / 2, bestNames.getGlobalBounds().height / 2);
	bestNames.setPosition(bestScoresText.getPosition().x-bestNames.getGlobalBounds().height / 2, bestScoresText.getPosition().y+100 );
	backText.setFont(f2);
	backText.setCharacterSize(40);
	backText.setFillColor(sf::Color::White);
	backText.setString("Back");
	backText.setOrigin(backText.getGlobalBounds().width / 2, backText.getGlobalBounds().height / 2);
	backText.setPosition(50, vm.height - 50);
	std::vector <std::tuple<int, std::string, std::string>> scoreVector;
	std::filesystem::directory_entry current_directory(std::filesystem::current_path() / "Scores");
	for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(current_directory))
	{
		auto name = entry.path().filename();
		for (const std::filesystem::directory_entry& entry2 : std::filesystem::directory_iterator(entry))
		{
			auto date = entry2.path().filename();
			std::ifstream filestream(entry2.path().string());
			if (filestream.is_open())
			{
				std::string score;
				while (std::getline(filestream, score))
				{
					if (score == "Score:")
					{
						if (std::getline(filestream, score))
						{
							std::string txt(".txt");
							std::string dateCut = date.string();
							auto found= dateCut.find(txt);
							if (found != std::string::npos)
								dateCut = dateCut.erase(found, txt.length());
							std::tuple <int, std::string, std::string> s = std::make_tuple(std::stoi(score), name.string(), dateCut);
							scoreVector.push_back(s);
						}
					}
				}
			}
		}
	}
	std::sort(scoreVector.begin(), scoreVector.end());
	auto reversed = std::ranges::views::reverse(scoreVector);
	std::vector <std::tuple<int, std::string, std::string>> toCut;
	std::ranges::for_each(reversed, [&toCut](auto score) {toCut.push_back(score); });
	auto cut = std::ranges::views::take(toCut, 5);
	std::string finalScores;
	std::ranges::for_each(cut, [&finalScores](auto score)
		{
			finalScores+= std::to_string(std::get<0>(score)) + "   " + std::get<1>(score) + "   " + std::get<2>(score) + "\n";
		});
	bestNames.setString(finalScores);
	while (true)
	{
		mousePosWindow = sf::Mouse::getPosition(*appWindowPtr);
		mousePosView = appWindowPtr->mapPixelToCoords(mousePosWindow);
		backText.setFillColor(sf::Color::White);
		if (backText.getGlobalBounds().contains(mousePosView))
		{
			backText.setFillColor(sf::Color::Red);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				appWindowPtr->clear();
				return;
			}
		}
		pollEvents();
		appWindowPtr->clear();
		appWindowPtr->draw(background);
		appWindowPtr->draw(bestScoresText);
		appWindowPtr->draw(bestNames);
		appWindowPtr->draw(backText);
		appWindowPtr->display();
	}
}

int App::getGamePhase()
{
	return gamePhase;
}

void App::setGamePhase(int phase)
{
	gamePhase = phase;
}

App::~App()
{
	delete this->appWindowPtr;
}
