#include <iostream>
#include <string>
#include <vector>
#include <cassert>

enum class accountType{
	Liability,
	Asset
};

enum class entryDirection{
	Credit,
	Debit
};

enum class entryStatus{
	Posted,
	Voided
};


struct account{
	std::string name;
	int id;
	accountType type;
	std::string created_at;
};

struct category{
	int id;
	int account_id;
	std::string name;
	bool is_active;
};

struct ledgerEntry{
	int id;
	int account_id;
	int category_id;
	double amount;
	entryDirection direction;
	std::string time_stamp;
	std::string description;
	entryStatus status;
};

class Ledger{
	public:
	 double balance(
	  const account& Account,
	  const std::vector<ledgerEntry>& entries,
          const std::string& upToTimeStamp
	 )const;

	 double total_credits(
	  int accountID,
	  const std::vector<ledgerEntry>& entries,
	  const std::string& start,
	  const std::string& end
	 )const;

	 double total_debits(
	  int accountID,
	  const std::vector<ledgerEntry>& entries,
	  const std::string& start,
	  const std::string& end
	 )const;

	 double total_category(
	  int accountID,
	  int categoryID,
	  entryDirection entry_direction,
	  const std::vector<ledgerEntry>& entries,
	  const std::string& start,
	  const std::string& end
	 )const;
};


constexpr int sign(accountType acc, entryDirection dir) noexcept{
	switch (acc){
		case accountType::Asset:
			return (dir == entryDirection::Debit) ? +1 : -1;
		case accountType::Liability:
			return (dir == entryDirection::Credit) ? +1 : -1; //had a little problem here LOL
	}
}
double Ledger::balance(
		const account& Account,
		const std::vector<ledgerEntry>& entries,
		const std::string& upToTimeStamp
		)
	const{
		double Balance = 0.0;
	for (const auto&e : entries){
		if (e.account_id != Account.id) continue;
		if (e.status != entryStatus::Posted) continue;
		if (e.time_stamp > upToTimeStamp) continue;
		
		Balance += e.amount * sign(Account.type, e.direction);
	}
           return Balance;
	}

double Ledger::total_credits(
		int accountID,
		const std::vector<ledgerEntry>& entries,
		const std::string& start,
		const std::string& end
		)
const{double total = 0.0;
	for (const auto&e : entries){
		if (e.account_id != accountID) continue;
		if (e.status != entryStatus::Posted) continue;
		if (e.direction != entryDirection::Credit) continue;
		if (e.time_stamp < start || e.time_stamp > end) continue;

		total += e.amount;
	}
	return total;
}

double Ledger::total_debits(
		int accountID,
		const std::vector<ledgerEntry>& entries,
		const std::string& start,
		const std::string& end
		)

	const{double total = 0.0;
		for (const auto&e : entries){
			if (e.account_id != accountID) continue;
			if (e.status != entryStatus::Posted) continue;
			if (e.direction != entryDirection::Debit) continue;
			if (e.time_stamp < start || e.time_stamp > end) continue;

			total += e.amount;

		}
		return total;
	}
double Ledger::total_category(
		int accountID,
		int categoryID,
		entryDirection entry_direction,
		const std::vector<ledgerEntry>& entries,
		const std::string& start,
		const std::string& end
		)
	const{double total = 0.0;

		for (const auto&e : entries){
			if (e.account_id != accountID) continue;
			if (e.category_id != categoryID) continue;
			if (e.direction != entry_direction) continue;
			if (e.status != entryStatus::Posted) continue;
			if (e.time_stamp < start || e.time_stamp > end) continue;

			total += e.amount;
		}
		return total;
	}

int main(){

	Ledger ledger;

	std::string start = "2025-01-01T00:00:00Z";
	std::string end = "2025-12-31T23:59:59Z";
	account account1{"account1", 1, accountType::Liability, "2025-01-03T23:21:23Z"};
	category net{1, 1, "net amount", true};
	category away{2, 1, "away from net", true};
	std::vector<ledgerEntry> entries = {
		{1, account1.id, net.id, 500.0, entryDirection::Credit, "2025-06-21T11:23:23Z", "salary", entryStatus::Posted},
		{2, account1.id, away.id, 210.0, entryDirection::Credit, "2025-07-23T22:12:31Z", "expenses", entryStatus::Posted}
	};

	std::cout<<ledger.balance(account1, entries, "2025-08-23T22:21:34Z") << "\n";
	std::cout<<ledger.total_credits(account1.id, entries, start, end) << "\n";
	std::cout<<ledger.total_debits(account1.id, entries, start, end) << "\n";
	std::cout<<ledger.total_category(account1.id, net.id, entryDirection::Credit, entries, start, end)<< "\n";

}
//NEXT: Add three dp fixed precision support (multiply user inputs by SF1000, use that for computation, then output divided by SF1000. 
//NEXT: Add formal currency support and actual time objects using chrono 
