/*
* File Name: Pong_Remake.cpp
* Author: Sebastian <3
* Date: 2016
* Purpose: Pong's main code
*/

#pragma once 

#include <iostream>
#include <sstream>
#include <Windows.h>
#include <cmath>
#include <cstdint>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\Window.hpp>

#include "Collision.hpp"
#include "Version.inc"

/*#define MODE */

int main()
{
	FreeConsole(); /* remove stupid console window */

	std::srand( static_cast<unsigned int>( std::time( NULL ) ) );

	bool isIdent = true;
	bool isPlaying = false;
	bool isPlayingBot = false;
	bool canSetTrue = true;
	bool canGetScore = false;

	static unsigned int mode = 0;
	static unsigned int collisions = 0;

	const float pi = 3.14159f;
	const unsigned int windowHeight = 900;
	const unsigned int windowWidth = 1400;
	const unsigned int bit = 32;

	bool lUp		= true;
	bool lDown		= true;
	bool rUp		= true;
	bool rDown		= true;

	bool isManual	= false;
	bool isScores    = false;
	bool isVersion   = false;

	sf::Sprite leftPaddle;
	sf::Sprite rightPaddle;
	sf::Texture paddleTexture;
	sf::Sprite ball;
	sf::Texture ballTexture;
	sf::Font font;
	sf::RectangleShape middleLine;
	sf::RectangleShape headLine;

	sf::SoundBuffer button;
	sf::SoundBuffer wall;
	sf::SoundBuffer paddle;
	sf::SoundBuffer death;

	sf::Sound btn;
	sf::Sound game;

	button.loadFromFile( std::string( "media/sounds/Button.wav" ) );
	wall.loadFromFile( std::string( "media/sounds/Wall.wav" ) );
	paddle.loadFromFile( std::string( "media/sounds/Paddle.wav" ) );
	death.loadFromFile( std::string( "media/sounds/Death.wav" ) );

	btn.setBuffer( button );

	sf::Text left;
	sf::Text right;

	/* menu system */
	sf::Text ident;
	sf::Text idversion;
	sf::Text idpress;

	sf::Text pause;
	sf::Text scores;
	sf::Text botScores;
	sf::Text manual;
	sf::Text botManual;
	sf::Text version;

	sf::Text back;

	std::stringstream v;
	std::stringstream d;
	std::stringstream t;

	v << VERSION;
	t << __TIME__;
	d << __DATE__;

	/* //////////// */

	sf::RenderWindow renderwindow( sf::VideoMode( windowWidth , windowHeight , bit ) , "Pong 2016 - Version: " VERSION " || Build: " __DATE__ " " __TIME__ " || Architecture: Windows (64 Bit)" , sf::Style::Titlebar | sf::Style::Close );
	renderwindow.setVerticalSyncEnabled( true );
	renderwindow.setMouseCursorVisible( false );

	sf::Image icon;
	icon.loadFromFile( std::string( "media/icons/WindowIcon.png" ) );
	renderwindow.setIcon( icon.getSize().x , icon.getSize().y , icon.getPixelsPtr() );

	paddleTexture.loadFromFile( std::string( "media/textures/Paddle.png" ) );
	ballTexture.loadFromFile( std::string( "media/textures/Ball.png" ) );
	font.loadFromFile( std::string( "media/fonts/SF Square Head.ttf" ) );

	ident.setFont( font );
	ident.setCharacterSize( 180 );
	ident.setColor( sf::Color::White );
	ident.setString( std::string( "PONG  2016" ) );
	ident.setStyle( sf::Text::Underlined );
	ident.setPosition( sf::Vector2f( 90 , 100 ) );
	ident.setScale( 1 , 1 );

	idversion.setFont( font );
	idversion.setCharacterSize( 40 );
	idversion.setColor( sf::Color::White );
	idversion.setString( std::string( "Version: " + v.str() ) );
	idversion.setPosition( sf::Vector2f( 350 , 340 ) );
	idversion.setScale( 1 , 1 );
	
	idpress.setFont( font );
	idpress.setCharacterSize( 51 );
	idpress.setColor( sf::Color::White );
	idpress.setString( std::string( "Press <SPACE> to start Game vs Human.\nPress <BACKSPACE> to start Game vs bot.\n" ) );
	idpress.setPosition( sf::Vector2f( 40 , 600 ) );
	idpress.setScale( 1 , 1 );

	manual.setFont( font );
	manual.setCharacterSize( 60 );
	manual.setColor( sf::Color::White );
	manual.setPosition( sf::Vector2f( 100 , 200 ) );
	manual.setStyle( sf::Text::Bold );
	manual.setString( std::string( "Manual:\n\nW - Left Paddle Up\nS - Left Paddle Down\n\nArrow Up - Right Paddle Up\nArrow Down - Right Paddle Down\n" ) );
	manual.setScale( 1 , 1 );

	botManual.setFont( font );
	botManual.setCharacterSize( 60 );
	botManual.setColor( sf::Color::White );
	botManual.setPosition( sf::Vector2f( 250 , 250 ) );
	botManual.setStyle( sf::Text::Bold );
	botManual.setString( std::string( "Manual:\n\nW - Left Paddle Up\nS - Left Paddle Down\n\nRight Paddle: Computer\n" ) );
	botManual.setScale( 1 , 1 );

	scores.setFont( font );
	scores.setCharacterSize( 60 );
	scores.setColor( sf::Color::White );
	scores.setPosition( sf::Vector2f( 200 , 270 ) );
	scores.setStyle( sf::Text::Bold );
	scores.setScale( 1 , 1 );

	botScores.setFont( font );
	botScores.setCharacterSize( 60 );
	botScores.setColor( sf::Color::White );
	botScores.setPosition( sf::Vector2f( 200 , 270 ) );
	botScores.setStyle( sf::Text::Bold );
	botScores.setScale( 1 , 1 );

	version.setFont( font );
	version.setCharacterSize( 60 );
	version.setColor( sf::Color::White );
	version.setPosition( sf::Vector2f( 50 , 240 ) );
	version.setStyle( sf::Text::Bold );
	version.setScale( 1 , 1 );
	version.setString( std::string( "Game Information:\n\nProgrammer: Sebastian <3\nResources: Sebastian <3, Web\nVersion: " + v.str() + "\nRelease: "+ d.str() + " | " + t.str() + "\nOriginal Concept: Allan Alcorn, 1972\n" ) );

	back.setFont( font );
	back.setCharacterSize( 40 );
	back.setColor( sf::Color::White );
	back.setPosition( sf::Vector2f( NULL + 30 , windowHeight - 70 ) );
	back.setStyle( sf::Text::Bold );
	back.setScale( 1 , 1 );
	back.setString( std::string( "Press <B> to switch Back to Pause Menu." ) );

	middleLine.setSize( sf::Vector2f( 4 , windowHeight - 300 ) );
	middleLine.setFillColor( sf::Color::White );
	middleLine.setOrigin( middleLine.getSize().x / 2 , middleLine.getSize().y / 2 );
	middleLine.setPosition( sf::Vector2f( windowWidth / 2 , windowHeight / 2 ) );

	leftPaddle.setTexture( paddleTexture );
	leftPaddle.setOrigin( paddleTexture.getSize().x / 2 , paddleTexture.getSize().y / 2 );
	leftPaddle.setPosition( NULL + 20 , windowHeight / 2 );
	leftPaddle.setScale( 0.85 , 1.4 );

	rightPaddle.setTexture( paddleTexture );
	rightPaddle.setOrigin( paddleTexture.getSize().x / 2 , paddleTexture.getSize().y / 2 );
	rightPaddle.setPosition( windowWidth - 20 , windowHeight / 2 );
	rightPaddle.setScale( 0.85 , 1.4 );

	ball.setTexture( ballTexture );
	ball.setOrigin( ballTexture.getSize().x / 2 , ballTexture.getSize().y / 2 );
	ball.setPosition( windowWidth / 2 , windowHeight / 2 );
	ball.setScale( 0.2 , 0.2 );

	pause.setFont( font );
	pause.setCharacterSize( 60 );
	pause.setColor( sf::Color::White );
	pause.setPosition( sf::Vector2f( 170 , 200 ) );
	pause.setStyle( sf::Text::Bold );
	pause.setString( std::string( "Game is paused:\n\nPress <R> to resume Game.\nPress <ESC> to quit Game.\nPress <H> to show Scores.\nPress <M> to show Manual.\nPress <V> to show Version.\nPress <X> to restart Game.\nPress <Q> to go to Main Menu.\n" ) );
	pause.setScale( 1 , 1 );

	left.setFont( font );
	left.setCharacterSize( 100 );
	left.setColor( sf::Color::White );
	left.setPosition( 300 , 30 );
	left.setStyle( sf::Text::Bold );
	left.setScale( 1 , 1 );

	right.setFont( font );
	right.setCharacterSize( 100 );
	right.setColor( sf::Color::White );
	right.setPosition( 1050 , 30 );
	right.setStyle( sf::Text::Bold );
	right.setScale( 1 , 1 );

	headLine.setSize( sf::Vector2f( 1000 , 5 ) );
	headLine.setFillColor( sf::Color::White );
	headLine.setPosition( 200 , 230 );

	const float paddleSpeed = 750.f;
	float botPaddleSpeed  = 0.f;
	const float ballSpeed   = 1200.f;
	float ballAngle         = 0.f;
	const float ballRadius	= 1.f;
	const sf::Time botTime   = sf::seconds(0.1f);

	const unsigned int max  = 220;
	const unsigned int min  = 50;

	sf::Clock clock;
	sf::Clock bot;

	int leftScore = 0;
	int rightScore = 0;

	while( renderwindow.isOpen() == true )
	{
		sf::Event mainevent;

		while( renderwindow.pollEvent( mainevent ) )
		{
			if( mainevent.type == sf::Event::Closed || ( mainevent.type == sf::Event::KeyPressed && mainevent.key.code == sf::Keyboard::Escape ) )
			{
				renderwindow.close();
				break;
			}

			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::F12 ) )
			{
				sf::Image screenshot = renderwindow.capture();

				sf::Uint64 randFileName = ( std::rand() % 10000000000000 );

				std::stringstream num;
				num << randFileName;

				screenshot.saveToFile( std::string( "screenshots/screenshot_" + num.str() + ".bmp" ) );
			}

			if( isPlaying == false && isPlayingBot == false )
			{
				if ( ( mainevent.type == sf::Event::KeyPressed ) && ( mainevent.key.code == sf::Keyboard::Space ) )
				{
					leftScore = 0;
					rightScore = 0;

					isPlaying = true;

					btn.play();

					#ifdef MODE 
						#define	MODE "HUMAN_GAME"
					#endif

					mode = 1;
					collisions = 0;
				}

				if( ( mainevent.type == sf::Event::KeyPressed ) && ( mainevent.key.code == sf::Keyboard::BackSpace ) )
				{
					leftScore = 0;
					rightScore = 0;

					isPlayingBot = true;

					btn.play();

					#ifdef MODE 
						#define MODE "BOT_GAME"	
					#endif

					mode = 2;
					collisions = 0;
				}
			}
		}

	if( isPlaying == true )
	{
		float deltaTime = clock.restart().asSeconds();

		if( leftPaddle.getPosition().y < max )
		{
			lUp = false;
		}

		else

		{
			lUp = true;
		}

		if( leftPaddle.getPosition().y > windowHeight - min )
		{
			lDown = false;
		}

		else

		{
			lDown = true;
		}


		if( rightPaddle.getPosition().y < max )
		{
			rUp = false;
		}

		else

		{
			rUp = true;
		}

		if( rightPaddle.getPosition().y > windowHeight - min )
		{
			rDown = false;
		}

		else

		{
			rDown = true;
		}

		std::stringstream l;
		l << leftScore;
		left.setString( l.str() );

		std::stringstream r;
		r << rightScore;
		right.setString( r.str() );

		/* replace score labels on scoreup to keep it in mid of balle field */

		if( ( leftScore == 10 || leftScore > 10 ) && leftScore < 100 )
		{
			left.setPosition( sf::Vector2f( 270 , 30 ) );
		}
		
		if( ( rightScore == 10 || rightScore > 10 ) && rightScore < 100  )
		{
			right.setPosition( sf::Vector2f( 1000 , 30 ) );
		}

		if( ( leftScore == 100 || leftScore > 100 ) && leftScore < 1000 )
		{
			left.setPosition( sf::Vector2f( 220 , 30 ) );
		}

		if( ( rightScore == 100 || rightScore > 100 ) && rightScore < 1000 )
		{
			right.setPosition( sf::Vector2f( 960 , 30 ) );
		}

		if( ( leftScore == 1000 || leftScore > 1000 ) && leftScore < 10000 )
		{
			left.setPosition( sf::Vector2f( 195 , 30 ) );
		}

		if( ( rightScore == 1000 || rightScore > 1000 ) && rightScore < 10000 )
		{
			right.setPosition( sf::Vector2f( 935 , 30 ) );
		}

		if( ( leftScore == 10000 || leftScore > 10000 ) && leftScore < 100000 )
		{
			left.setPosition( sf::Vector2f( 170 , 30  ) );
		}

		if( ( rightScore == 10000 || rightScore > 10000 ) && rightScore < 100000 )
		{
			right.setPosition( sf::Vector2f( 910 , 30 ) );
		}

		/* //////////////////////////////////////////////////////////////////////////////////// */


		/* move player 1's paddle */
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::W ) && lUp == true )
		{
			leftPaddle.move( NULL , -paddleSpeed * deltaTime );
		}

		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::S ) && lDown == true )
		{
			leftPaddle.move( NULL , paddleSpeed * deltaTime );
		}

		/* move player 2's paddle */
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Up ) && rUp == true )
		{
			rightPaddle.move( NULL , -paddleSpeed * deltaTime );
		}

		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Down ) && rDown == true )
		{
			rightPaddle.move( NULL , paddleSpeed * deltaTime );
		}

		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::P ) )
		{
			canGetScore = false;

			isPlaying = false;
			renderwindow.setTitle( std::string( "Cocoz' Pong - Version: " VERSION " || Build: " __DATE__ " " __TIME__ " || Architecture: Windows (64 Bit)" ) );
			btn.play();
		}

	    float factor = ballSpeed * deltaTime;
        ball.move( std::cos( ballAngle ) * factor, std::sin( ballAngle ) * factor );

		if( canGetScore == true )
		{
			if ( ball.getPosition().x - ballRadius < 0.f )
			{ 
				 rightScore = ( rightScore + 1 );
				 ball.setPosition( windowWidth / 2, windowHeight / 2 );

				 /*
				 game.setBuffer( death );
				 game.play();
				 */
			}

			if ( ball.getPosition().x + ballRadius > windowWidth )
			{
				 leftScore = ( leftScore + 1 );
				 ball.setPosition( windowWidth / 2, windowHeight / 2 );

				 /* 
				 game.setBuffer( death );
				 game.play();
				 */
			}
		}

		const unsigned int ballMax = max - 40;

		if ( ball.getPosition().y - ballRadius < ballMax )
        {
            ballAngle = -ballAngle;
            ball.setPosition(ball.getPosition().x, ballRadius + ballMax );

			game.setBuffer( wall );
			game.play();
        }

        if ( ball.getPosition().y + ballRadius > windowHeight )
        {
            ballAngle = -ballAngle;
            ball.setPosition( ball.getPosition().x, windowHeight - ballRadius - 0.1f );

			game.setBuffer( wall );
			game.play();
        }

		if( Collision::PixelPerfectTest( leftPaddle , ball ) )
		{
			 if ( ball.getPosition().y > leftPaddle.getPosition().y )
			 {
				 ballAngle = pi - ballAngle + ( std::rand() % 20 ) * pi / 180;
			 }
			 
			 else  

			 {
				 ballAngle = pi - ballAngle - ( std::rand() % 20 ) * pi / 180;
			 }

			 collisions = ( collisions + 1 );

			 game.setBuffer( paddle );
			 game.play();
		}

		if( Collision::PixelPerfectTest( rightPaddle , ball ) )
		{
			if ( ball.getPosition().y > rightPaddle.getPosition().y )
			{
				ballAngle = pi - ballAngle + ( std::rand() % 20 ) * pi / 180;
			}

			else

			{
				ballAngle = pi - ballAngle - ( std::rand() % 20 ) * pi / 180;
			}

			collisions = ( collisions + 1 );

			game.setBuffer( paddle );
			game.play();
		}
	}



	// ////////////////////////////////////////////////////////////////////////////////////////////////////// //
	/* bot control */

	if( isPlayingBot == true )
	{
		float deltaTime = clock.restart().asSeconds();

		if( leftPaddle.getPosition().y < max )
		{
			lUp = false;
		}

		else

		{
			lUp = true;
		}

		if( leftPaddle.getPosition().y > windowHeight - min )
		{
			lDown = false;
		}

		else

		{
			lDown = true;
		}


		if( rightPaddle.getPosition().y < max )
		{
			rUp = false;
		}

		else

		{
			rUp = true;
		}

		if( rightPaddle.getPosition().y > windowHeight - min )
		{
			rDown = false;
		}

		else

		{
			rDown = true;
		}

		std::stringstream l;
		l << leftScore;
		left.setString( l.str() );

		std::stringstream r;
		r << rightScore;
		right.setString( r.str() );

		/* replace score labels on scoreup to keep it in mid of balle field */

		if( ( leftScore == 10 || leftScore > 10 ) && leftScore < 100 )
		{
			left.setPosition( sf::Vector2f( 270 , 30 ) );
		}
		
		if( ( rightScore == 10 || rightScore > 10 ) && rightScore < 100  )
		{
			right.setPosition( sf::Vector2f( 1000 , 30 ) );
		}

		if( ( leftScore == 100 || leftScore > 100 ) && leftScore < 1000 )
		{
			left.setPosition( sf::Vector2f( 220 , 30 ) );
		}

		if( ( rightScore == 100 || rightScore > 100 ) && rightScore < 1000 )
		{
			right.setPosition( sf::Vector2f( 960 , 30 ) );
		}

		if( ( leftScore == 1000 || leftScore > 1000 ) && leftScore < 10000 )
		{
			left.setPosition( sf::Vector2f( 195 , 30 ) );
		}

		if( ( rightScore == 1000 || rightScore > 1000 ) && rightScore < 10000 )
		{
			right.setPosition( sf::Vector2f( 935 , 30 ) );
		}

		if( ( leftScore == 10000 || leftScore > 10000 ) && leftScore < 100000 )
		{
			left.setPosition( sf::Vector2f( 170 , 30  ) );
		}

		if( ( rightScore == 10000 || rightScore > 10000 ) && rightScore < 100000 )
		{
			right.setPosition( sf::Vector2f( 910 , 30 ) );
		}

		/* //////////////////////////////////////////////////////////////////////////////////// */


		/* move player 1's paddle */
		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::W ) && lUp == true )
		{
			leftPaddle.move( NULL , -paddleSpeed * deltaTime );
		}

		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::S ) && lDown == true )
		{
			leftPaddle.move( NULL , paddleSpeed * deltaTime );
		}

		 /* Move the computer's paddle */

            if ( ( ( botPaddleSpeed < 0.f ) && ( rightPaddle.getPosition().y - paddleTexture.getSize().y / 2 > 5.f ) ) || ( ( botPaddleSpeed > 0.f ) && ( rightPaddle.getPosition().y + paddleTexture.getSize().y / 2 < windowHeight - 20 ) ) )
            {
                rightPaddle.move( 0.f, botPaddleSpeed * deltaTime );
            }

            /* move computer paddle as ball */

            if ( bot.getElapsedTime() > botTime )
            {
                bot.restart();

				if( ball.getPosition().y + ballRadius > rightPaddle.getPosition().y + paddleTexture.getSize().y / 2 )
				{
                    botPaddleSpeed = paddleSpeed;
				}

                else if( ball.getPosition().y - ballRadius < rightPaddle.getPosition().y - paddleTexture.getSize().y / 2 )
				{
                    botPaddleSpeed = -paddleSpeed;
				}

                else

				{
                    botPaddleSpeed = 0.f;
				}
            }

		if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::P ) )
		{
			canGetScore = false;

			isPlayingBot = false;
			renderwindow.setTitle( std::string( "Cocoz' Pong - Version: " VERSION " || Build: " __DATE__ " " __TIME__ " || Architecture: Windows (64 Bit)" ) );
			btn.play();
		}

	    float factor = ballSpeed * deltaTime;
        ball.move( std::cos( ballAngle ) * factor, std::sin( ballAngle ) * factor );

		if( canGetScore == true )
		{
			if ( ball.getPosition().x - ballRadius < 0.f )
			{ 
				 rightScore = ( rightScore + 1 );
				 ball.setPosition( windowWidth / 2, windowHeight / 2 );

				 /*
				 game.setBuffer( death );
				 game.play();
				 */
			}

			if ( ball.getPosition().x + ballRadius > windowWidth )
			{
				 leftScore = ( leftScore + 1 );
				 ball.setPosition( windowWidth / 2, windowHeight / 2 );

				 /* 
				 game.setBuffer( death );
				 game.play();
				 */
			}
		}

		const unsigned int ballMax = max - 40;

		if ( ball.getPosition().y - ballRadius < ballMax )
        {
           /* ballSound.play(); */
            ballAngle = -ballAngle;
            ball.setPosition(ball.getPosition().x, ballRadius + ballMax );

			game.setBuffer( wall );
			game.play();
        }

        if ( ball.getPosition().y + ballRadius > windowHeight )
        {
            //ballSound.play();
            ballAngle = -ballAngle;
            ball.setPosition( ball.getPosition().x, windowHeight - ballRadius - 0.1f );

			game.setBuffer( wall );
			game.play();
        }

		if( Collision::PixelPerfectTest( leftPaddle , ball ) )
		{
			 if ( ball.getPosition().y > leftPaddle.getPosition().y )
			 {
				 ballAngle = pi - ballAngle + ( std::rand() % 20 ) * pi / 180;
			 }
			 
			 else  

			 {
				 ballAngle = pi - ballAngle - ( std::rand() % 20 ) * pi / 180;
			 }

			 collisions = ( collisions + 1 );

			 game.setBuffer( paddle );
			 game.play();
		}

		if( Collision::PixelPerfectTest( rightPaddle , ball ) )
		{
			if ( ball.getPosition().y > rightPaddle.getPosition().y )
			{
				ballAngle = pi - ballAngle + ( std::rand() % 20 ) * pi / 180;
			}

			else

			{
				ballAngle = pi - ballAngle - ( std::rand() % 20 ) * pi / 180;
			}

			collisions = ( collisions + 1 );

			game.setBuffer( paddle );
			game.play();
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////
	/* end of bot control */
	///////////////////////////////////////////////////////////////////////////////////////
	

	renderwindow.clear( sf::Color::Black );

		if( isIdent == true )
		{
			renderwindow.draw( ident );
			renderwindow.draw( idversion );
			renderwindow.draw( idpress );
		}

		if ( isPlayingBot == true )
        {
			isIdent = false;

			renderwindow.draw( middleLine );
			//renderwindow.draw( headLine );
            renderwindow.draw( leftPaddle );
            renderwindow.draw( rightPaddle );
			renderwindow.draw( left );
			renderwindow.draw( right );
			renderwindow.draw( ball );

			canGetScore = true;
		}

        if ( isPlaying == true )
        {
			isIdent = false;

			renderwindow.draw( middleLine );
			//renderwindow.draw( headLine );
            renderwindow.draw( leftPaddle );
            renderwindow.draw( rightPaddle );
			renderwindow.draw( left );
			renderwindow.draw( right );
			renderwindow.draw( ball );

			canGetScore = true;
		}

		else if( isPlaying == false && isIdent == false && isPlayingBot == false )

		{
			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::R ) )
			{
				if( isManual == false && isScores == false && isVersion == false )
				{
					if( mode == 1 )
					{
						isPlaying = true;
						renderwindow.setTitle( std::string( "Cocoz' Pong - Version: " VERSION " || Build: " __DATE__ " " __TIME__ " || Architecture: Windows (64 Bit)" ) );
						canGetScore = true;
					}

					else if( mode == 2 )
					
					{
						isPlayingBot = true;
						renderwindow.setTitle( std::string( "Cocoz' Pong - Version: " VERSION " || Build: " __DATE__ " " __TIME__ " || Architecture: Windows (64 Bit)" ) );
						canGetScore = true;
					}

					btn.play();
				}
			}

			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::M ) )
			{
				if( isManual == false && isScores == false && isVersion == false && isPlayingBot == false )
				{
					isManual = true;
					btn.play();
				}
			}

			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::B ) )
			{
				if( isScores == true || isVersion == true || isManual == true )
				{
					isScores = false;
					isVersion = false;
					isManual = false;
					btn.play();
				}
			}

			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::H ) )
			{
				if( isScores == false && isManual == false && isVersion == false )
				{
					isScores = true;
					btn.play();
				}
			}

			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::V ) )
			{
				if( isScores == false && isManual == false && isVersion == false )
				{
					isVersion = true;
					btn.play();
				}
			}

			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::Q ) )
			{
				isVersion = false;
				isManual = false;
				isScores = false;

				if( mode == 1 )
				{
					isPlaying = false;
				}

				else if( mode == 2 )
				{
					isPlayingBot = false;
				}

				leftScore = 0;
				rightScore = 0;
				collisions = 0;

				lUp		= true;
				lDown	= true;
				rUp		= true;
				rDown	= true;

				leftPaddle.setPosition( NULL + 20 , windowHeight / 2 );
				rightPaddle.setPosition( windowWidth - 20 , windowHeight / 2 );
				ball.setPosition( windowWidth / 2 , windowHeight / 2 );

				ballAngle = 0.f;
				botPaddleSpeed = 0.f;

				isIdent = true;

				btn.play();
			}

			if( sf::Keyboard::isKeyPressed( sf::Keyboard::Key::X ) )
			{
				if( isScores == false && isManual == false && isVersion == false )
				{
					if( mode == 1 )
					{
						leftScore = 0;
						rightScore = 0;
						collisions = 0;
						leftPaddle.setPosition( NULL + 20 , windowHeight / 2 );
						rightPaddle.setPosition( windowWidth - 20 , windowHeight / 2 );
						ball.setPosition( windowWidth / 2 , windowHeight / 2 );

						lUp		= true;
						lDown	= true;
						rUp		= true;
						rDown	= true;

						isPlaying = true;
						btn.play();
					}

					else if( mode == 2 )
					{
						leftScore = 0;
						rightScore = 0;
						collisions = 0;
						leftPaddle.setPosition( NULL + 20 , windowHeight / 2 );
						ball.setPosition( windowWidth / 2 , windowHeight / 2 );
						rightPaddle.setPosition( windowWidth - 20 , windowHeight / 2 );
						
						lUp		= true;
						lDown	= true;
						rUp		= true;
						rDown	= true;
						
						isPlayingBot = true;
						btn.play();
					}

						ballAngle = 0.f;
						botPaddleSpeed = 0.f;
				}
			}

			if( isVersion == false && isManual == false && isScores == false )
			{
				renderwindow.draw( pause );
			}

			if( isManual == true )
			{
				if( mode == 1 )
				{
					renderwindow.draw( manual );
				}

				else if( mode == 2 )
				{
					renderwindow.draw( botManual );
				}
			}

			if( isScores == true )
			{
				std::stringstream leftS;
				std::stringstream rightS;
				std::stringstream collision;

				leftS << leftScore;
				rightS << rightScore;
				collision << collisions;

				scores.setString( std::string( "Current Gamescore:\n\nPlayer 1 (L):  " + leftS.str() + "\nPlayer 2 (R):  " + rightS.str() + "\nCollisions:      " + collision.str() + "\n" ) );

				renderwindow.draw( scores );
			}

			if( isVersion == true )
			{
				renderwindow.draw( version );
			}

			if( isVersion == true || isManual == true || isScores == true )
			{
				renderwindow.draw( back );
			}
		}

		renderwindow.display();
	}

	return EXIT_SUCCESS;
}
