#include <iostream>
#include <string>
#include <chrono>
#include <vector>

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
          const std::string& upToTimeStamp,
	 )const;

	 double total_credits(
	  int accountID,
	  const std::vector<ledgerEntry>& entries,
	  const std::string& start,
	  const std::string& end
	 )const;

	 double total_debits(
	  int accountID,
	  const std::vector<ledgerEntry>% entries,
	  const std::string& start,
	  const std::string& end
	 )const;

	 double total_category(
	  int accountID,
	  int categoryID,
	  entryDirection entry_direction,
	  const std::vector<ledgerEntry>% entries,
	  const std::string& start,
	  const std::string& end
	 )const;
};

double Ledger::balance(
		const account& Account,
		const std::vector<ledgerEntry>& entries,
		const std::string& upToTimeStamp
		)
	const{
		double Balance = 0.0;
	for (const auto&e : entries){
		if (e.account_id != account.id) continue;
		if (e.status != entryStatus::Posted) continue;
		if (e.timestamp > upToTimeStamp) continue;
		if (e.direction == entryDirection::Credit){
			Balance += e.amount;
		}
		else{
			Balance -= e.amount;
		}
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
		if (e.account_id != account.id) continue;
		if (e.status != entryStatus::Posted) continue;
		if (e.direction != entryDirection::Credit) continue;
		if (e.timestamp < start || e.timestamp > end) continue;

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
			if (e.account_id != account.id) continue;
			if (e.status != entryStatus::Posted) continue;
			if (e.direction != entryDirection::Debit) continue;
			if (e.timestamp < start || e.timestamp > end) continue;

			total += e.amount;

		}
		return total;
	}

//add Ledger::total_category
//this is not executable or compilable yet. No main().
