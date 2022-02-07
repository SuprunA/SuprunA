#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>

class Card
{
public:
    enum rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };
    enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };

    friend std::ostream& operator<<(std::ostream& os, const Card& aCard)
    {
        const std::string RANKS[] = { "0", "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
        const std::string SUITS[] = { "c", "d", "h", "s" };

        if (aCard.m_IsFaceUp)
            os << RANKS[aCard.m_Rank] << SUITS[aCard.m_Suit];
        else
            os << "XX";

        return os;
    }

    Card(rank r = ACE, suit s = SPADES, bool ifu = true) : m_Rank(r), m_Suit(s), m_IsFaceUp(ifu) {}

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
        m_IsFaceUp = !(m_IsFaceUp);
    }

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
            *iter = 0;
        }
        m_Cards.clear();
    }

    int GetTotal() const
    {
        if (m_Cards.empty())
            return 0;

        if (m_Cards[0]->GetValue() == 0)
            return 0;

        int total = 0;
        std::vector<Card*>::const_iterator iter;
        for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
            total += (*iter)->GetValue();

        bool containsAce = false;
        for (iter = m_Cards.begin(); iter != m_Cards.end(); ++iter)
        {
            if ((*iter)->GetValue() == Card::ACE)
                containsAce = true;
        }

        if (containsAce && total <= 11)
            total += 10;

        return total;
    }

protected:
    std::vector<Card*> m_Cards;
};

class GenericPlayer : public Hand
{
    friend std::ostream& operator<<(std::ostream& os, const GenericPlayer& aGenericPlayer)
    {
        os << aGenericPlayer.m_Name << ":\t";

        std::vector<Card*>::const_iterator pCard;
        if (!aGenericPlayer.m_Cards.empty())
        {
            for (pCard = aGenericPlayer.m_Cards.begin(); pCard != aGenericPlayer.m_Cards.end(); ++pCard)
                os << *(*pCard) << "\t";

            if (aGenericPlayer.GetTotal() != 0)
                std::cout << "(" << aGenericPlayer.GetTotal() << ")";
        }
        else
            os << "<empty>";

        return os;
    }

public:
    GenericPlayer(const std::string& name = "") : m_Name(name) {}

    virtual ~GenericPlayer() {}

    virtual bool IsHitting() const = 0;

    bool IsBusted() const
    {
        return (GetTotal() > 21);
    }

    void Bust() const
    {
        std::cout << m_Name << " busts.\n";
    }

protected:
    std::string m_Name;
};

class Player : public GenericPlayer
{
public:
    Player(const std::string& name = "") : GenericPlayer(name) {}

    virtual ~Player() {}

    virtual bool IsHitting() const
    {
        std::cout << m_Name << ", do you want a hit? (Y/N): ";
        char response;
        std::cin >> response;
        return (response == 'y' || response == 'Y');
    }

    void Win() const
    {
        std::cout << m_Name << " wins.\n";
    }

    void Lose() const
    {
        std::cout << m_Name << " loses.\n";
    }

    void Push() const
    {
        std::cout << m_Name << " pushes.\n";
    }
};

class House : public GenericPlayer
{
public:
    House(const std::string& name = "House") : GenericPlayer(name) {}

    virtual ~House() {}

    virtual bool IsHitting() const
    {
        return (GetTotal() <= 16);
    }

    void FlipFirstCard()
    {
        if (!(m_Cards.empty()))
            m_Cards[0]->Flip();
        else
            std::cout << "No card to flip!\n";
    }
};

class Deck : public Hand
{
public:
    Deck()
    {
        m_Cards.reserve(52);
        Populate();
    }

    virtual ~Deck() {}

    void Populate()
    {
        Clear();
        for (int s = Card::CLUBS; s <= Card::SPADES; ++s)
        {
            for (int r = Card::ACE; r <= Card::KING; ++r)
                Add(new Card(static_cast<Card::rank>(r), static_cast<Card::suit>(s)));
        }
    }

    void Shuffle()
    {
        random_shuffle(m_Cards.begin(), m_Cards.end());
    }

    void Deal(Hand& aHand)
    {
        if (!m_Cards.empty())
        {
            aHand.Add(m_Cards.back());
            m_Cards.pop_back();
        }
        else
            std::cout << "Out of cards. Unable to deal.";
    }

    void AdditionalCards(GenericPlayer& aGenericPlayer)
    {
        std::cout << std::endl;
        while (!(aGenericPlayer.IsBusted()) && aGenericPlayer.IsHitting())
        {
            Deal(aGenericPlayer);
            std::cout << aGenericPlayer << std::endl;

            if (aGenericPlayer.IsBusted())
                aGenericPlayer.Bust();
        }
    }
};

class Game
{
public:
    Game(const std::vector<std::string>& names)
    {
        std::vector<std::string>::const_iterator pName;
        for (pName = names.begin(); pName != names.end(); ++pName)
        {
            m_Players.push_back(Player(*pName));
        }

        srand(static_cast<unsigned int>(time(0)));
        m_Deck.Populate();
        m_Deck.Shuffle();
    }

    ~Game() {}

    void Play()
    {
        std::vector<Player>::iterator pPlayer;
        for (int i = 0; i < 2; ++i)
        {
            for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
                m_Deck.Deal(*pPlayer);

            m_Deck.Deal(m_House);
        }

        m_House.FlipFirstCard();

        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
            std::cout << *pPlayer << std::endl;

        std::cout << m_House << std::endl;

        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
            m_Deck.AdditionalCards(*pPlayer);

        m_House.FlipFirstCard();
        std::cout << std::endl << m_House;

        m_Deck.AdditionalCards(m_House);

        if (m_House.IsBusted())
        {
            for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
            {
                if (!(pPlayer->IsBusted()))
                    pPlayer->Win();
            }
        }
        else
        {
            for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
            {
                if (!(pPlayer->IsBusted()))
                {
                    if (pPlayer->GetTotal() > m_House.GetTotal())
                        pPlayer->Win();
                    else if (pPlayer->GetTotal() < m_House.GetTotal())
                        pPlayer->Lose();
                    else
                        pPlayer->Push();
                }
            }

        }

        for (pPlayer = m_Players.begin(); pPlayer != m_Players.end(); ++pPlayer)
            pPlayer->Clear();

        m_House.Clear();
    }

private:
    Deck m_Deck;
    House m_House;
    std::vector<Player> m_Players;
};

void exercise_5();

class Date
{
private:
	int m_day;
	int m_month;
	int m_year;
public:
	Date(int day, int month, int year) : m_day(day), m_month(month), m_year(year) {}

	const int& getDay() const
	{
		return m_day;
	}

	const int& getMonth() const
	{
		return m_month;
	}

	const int& getYear() const
	{
		return m_year;
	}

	friend std::ostream& operator<<(std::ostream& os, const Date& d)
	{
		os << d.m_day << '.' << d.m_month << '.' << d.m_year;

		return os;
	}
};

template <class T>
class _ptr
{
private:
	T* m_ptr;
public:
	_ptr(T* ptr = nullptr) : m_ptr(ptr) {}
	~_ptr() { delete m_ptr; }

	_ptr(_ptr& p)
	{
		m_ptr = p.m_ptr;
		p.m_ptr = nullptr;
	}

	_ptr& operator=(_ptr& p)
	{
		if (&p == this)
			return *this;

		delete m_ptr;
		m_ptr = p.m_ptr;
		p.m_ptr = nullptr;

		return *this;
	}

	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }

	bool isNull() const { return m_ptr == nullptr; }
};

const _ptr<Date>& compares_dates(const _ptr<Date>&, const _ptr<Date>&);

void exercise_1();
void exercise_2();

void exercise_1()
{
    _ptr<Date> today(new Date(10, 07, 2021));

    std::cout << "Day: " << today->getDay() << std::endl;
    std::cout << "Month: " << today->getMonth() << std::endl;
    std::cout << "Year: " << today->getYear() << std::endl;
    std::cout << "today: " << *today << std::endl;

    _ptr<Date> date;

    std::cout << "today is " << (today.isNull() ? "null\n" : "not null\n");
    std::cout << "date is " << (date.isNull() ? "null\n" : "not null\n");

    date = today;

    std::cout << "today is " << (today.isNull() ? "null\n" : "not null\n");
    std::cout << "date is " << (date.isNull() ? "null\n" : "not null\n");

    std::cout << "date: " << *date << std::endl;
}

const _ptr<Date>& compares_dates(const _ptr<Date>& a, const _ptr<Date>& b)
{
    if (a->getYear() > b->getYear())
        return a;
    else if (a->getYear() < b->getYear())
        return b;
    else
    {
        if (a->getMonth() > b->getMonth())
            return a;
        else if (a->getMonth() < b->getMonth())
            return b;
        else
        {
            if (a->getDay() > b->getDay())
                return a;
            else
                return b;
        }
    }
}

void swap_dates(_ptr<Date>& a, _ptr<Date>& b)
{
    _ptr<Date> temp(a);
    a = b;
    b = temp;
}

void exercise_2()
{
    _ptr<Date> date1(new Date(9, 07, 2022));
    _ptr<Date> date2(new Date(10, 07, 2022));
    _ptr<Date> date3(new Date(11, 07, 2022));

    std::cout << *compares_dates(date1, date2) << std::endl;
    swap_dates(date2, date3);
    std::cout << *compares_dates(date1, date2) << std::endl;
}

void exercise_5()
{
    std::cout << "\t\tWelcome to Blackjack!\n\n";

    int numPlayers = 0;
    while (numPlayers < 1 || numPlayers > 7)
    {
        std::cout << "How many players? (1 - 7): ";
        std::cin >> numPlayers;
    }

    std::vector<std::string> names;
    std::string name;
    for (int i = 0; i < numPlayers; ++i)
    {
        std::cout << "Enter player name: ";
        std::cin >> name;
        names.push_back(name);
    }
    std::cout << std::endl;

    //the game loop
    Game aGame(names);
    char again = 'y';
    while (again != 'n' && again != 'N')
    {
        aGame.Play();
        std::cout << "\nDo you want to play again? (Y/N): ";
        std::cin >> again;
    }
}

int main()
{
    exercise_1();
    exercise_2();
    exercise_5();

    return 0;
}
