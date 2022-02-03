#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class Card
{
public:
	enum rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
	enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };

	Card(suit s, bool ifu = true, rank r = ACE ) : m_Rank(r), m_Suit(s), m_IsFaceUp(ifu) { }

	int GetValue() const
	{
		int value = 0;
		if (m_IsFaceUp)
		{
			value = m_Rank;
			if (value > 10)
				value = 10;
		}
		return value;
	}

	void Flip()
	{
		m_IsFaceUp = !m_IsFaceUp;
	}

	friend std::ostream& operator<<(std::ostream& s, const Card& aCard);

private:
	rank m_Rank;
	suit m_Suit;
	bool m_IsFaceUp;
};

class Hand
{
public:
	Hand()
	{
		m_Cards.reserve(7);
	}

	virtual ~Hand()
	{
		Clear();
	}

	void Add(Card* pCard)
	{
		m_Cards.push_back(pCard);
	}

	void Clear()
	{
		std::vector<Card*>::iterator iter = m_Cards.begin();
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
		{
			delete* iter;
			*iter = nullptr;
		}

		m_Cards.clear();
	}

	int GetTotal() const
	{
		if (m_Cards.empty() || m_Cards[0]->GetValue() == 0)
			return 0;

		int total = 0;
		std::vector<Card*>::const_iterator iter;

		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
			total += (*iter)->GetValue();

		bool containsAce = false;
		for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
			if ((*iter)->GetValue() == Card::ACE)
				containsAce = true;

		if (containsAce && total <= 11) total += 10;

		return total;
	}

protected:
	std::vector<Card*> m_Cards;
};

class GenericPlayer : public Hand
{
private:
	friend ostream& operator<<(ostream& os, const GenericPlayer& aGenericPlayer);
protected:
	string m_Name;
public:
	GenericPlayer(const string& name = "") : m_Name(name) { }

	virtual ~GenericPlayer();

	virtual bool IsHitting() const = 0;

	bool IsBusted() const
	{
		return (GetTotal() > 21);
	}

	void Bust() const
	{
		cout << m_Name << " перебор.\n";
	}
};

void exercise_1()
{
	int num;

	std::cout << "Введите целое число: ";

	while (true)
	{
		std::cin >> num;

		if (std::cin.good() && std::cin.peek() == '\n')
		{
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			break;
		}
		std::cin.clear();
		std::cout << "Введено некорректное число. Попробуйте ещё раз: ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	std::cout << "Введено целое число " << num << std::endl;
}

//class endline
//{
//public:
//	friend std::ostream& operator<<(std::ostream &out, const endline&);
//};
//
//std::ostream& operator<<(std::ostream &out, const endline& e)
//{
//  out << "\n\n";
//  out.flush();
//
//  return out;
//}
//
//endline endll;

template<typename _CharT, typename _Traits>

inline std::basic_ostream<_CharT, _Traits>&

endll(std::basic_ostream<_CharT, _Traits>& __os)
{
	return flush((__os.put(__os.widen('\n'))).put(__os.widen('\n')));
}

void exercise_2()
{
	std::cout << "Hello" << endll << "world";
}

class Player : public GenericPlayer
{
public:
	Player(const string& name = "") : GenericPlayer(name) { }

	virtual ~Player() { }

	virtual bool IsHitting() const
	{
		cout << m_Name << ", do you want a hit? (Y/N): ";
		char response;
		cin >> response;
		return (response == 'y' || response == 'Y');
	}

	void Win() const
	{
		cout << m_Name << " wins.\n";
	}

	void Lose() const
	{
		cout << m_Name << " loses.\n";
	}

	void Push() const
	{
		cout << m_Name << " pushes.\n";
	}
};

class House : public GenericPlayer
{
public:
	House(const string& name = "House") : GenericPlayer(name) { }

	virtual ~House() { }

	virtual bool IsHitting() const
	{
		return (GetTotal() <= 16);
	}

	void FlipFirstCard()
	{
		if (!(m_Cards.empty()))
			m_Cards[0]->Flip();
		else
			cout << "Нет карты для переворота!\n";
	}
};

ostream& operator<<(ostream& os, const Card& aCard)
{
	const string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9",
							"10", "J", "Q", "K" };
	const string SUITS[] = { "c", "d", "h", "s" };

	if (aCard.m_IsFaceUp)
		os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
	else
		os << "XX";

	return os;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	exercise_1();
	exercise_2();

	return 0;
}
