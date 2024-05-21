#pragma once
#include <string>
#include <thread>
#include <Circle.hpp>

namespace mt
{

	const float pi = acos(-1);

	class Game
	{
		int m_width;
		int m_height;
		std::string m_capture;
		mt::Circle* m_c;
		int m_n;
		sf::RenderWindow m_window;

	public:
		Game(int width, int height, const std::string& capture) :
			m_width(width), m_height(height), m_capture(capture), m_c(nullptr), m_n(0) {}

		void Setup(int n)
		{
			m_n = n;

			// Ñîçäàíèå îêíà
			m_window.create(sf::VideoMode(m_width, m_height), m_capture);

			srand(time(0));

			m_c = new mt::Circle[m_n];
			for (int i = 0; i < m_n; i++)
			{
				int r = rand() % 100 + 1;
				int x = rand() % (1000 - 2 * r) + r;
				int y = rand() % (600 - 2 * r) + r;
				int v = rand() % 50 + 10;
				float alfa = rand() % 7;
				m_c[i].Setup(x, y, r, v, alfa);
			}
		}

		void TouchBorder(Circle& obj)
		{
			float x = obj.X();
			float y = obj.Y();
			float r = obj.R();

			if (x + r >= m_width || x - r <= 0)
			{
				obj.Alfa(pi - obj.Alfa());
			}

			if (y + r >= m_height || y - r <= 0)
			{
				obj.Alfa(2 * pi - obj.Alfa());
			}

		}

		// Êîëëèçèÿ
		void CheckCollisions() {
			for (int i = 0; i < m_n - 1; i++)
			{
				for (int j = i + 1; j < m_n; j++)
				{
					float dx = m_c[i].X() - m_c[j].X();
					float dy = m_c[i].Y() - m_c[j].Y();
					float distance = sqrt(dx * dx + dy * dy);


					if (distance < m_c[i].R() + m_c[j].R())
					{

						float angle_i = atan2(dy, dx);

						float angle_j = atan2(-dy, -dx);

						m_c[i].Alfa(angle_i);

						m_c[j].Alfa(angle_j);

					}
				}
			}
		}

		void LifeCycle()
		{
			sf::Clock clock;

			while (m_window.isOpen())
			{
				// Îáðàáîòêà ñîáûòèé
				sf::Event event;
				while (m_window.pollEvent(event))
				{
					if (event.type == sf::Event::Closed)
						m_window.close();
				}


				// Ëîãèêà
				float dt = clock.getElapsedTime().asSeconds();
				clock.restart();

				// Ïåðåìåùåíèå øàðèêà
				for (int i = 0; i < m_n; i++) {
					m_c[i].Move(dt);
				}

				for (int i = 0; i < m_n; i++)
					TouchBorder(m_c[i]);

				CheckCollisions();

				// Îòîáðàæåíèå
				m_window.clear();
				for (int i = 0; i < m_n; i++)
					m_window.draw(m_c[i].Get());
				m_window.display();
			}
		}
	};

}