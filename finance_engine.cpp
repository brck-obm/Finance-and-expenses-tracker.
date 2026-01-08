#include <iostream>
#include <string>
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

	 double total_category( //this answers how much money, in which direction of this category, during a given time.
	  int accountID,
	  int categoryID,
	  entryDirection entry_direction,
	  const std::vector<ledgerEntry>& entries,
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
		if (e.account_id != Account.id) continue;
		if (e.status != entryStatus::Posted) continue;
		if (e.time_stamp > upToTimeStamp) continue;
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

//main entry point 
int main(){
	//test cases
account my_account{"my_account", 1, accountType::Asset, "2024-01-08T08:29:00Z"}; //ISO8601 time (YYYY-MM-DDTHH-mm-SSZ)
category salary{1, 1, "Salary", true};
category groceries{2, 1, "Groceries", true};

std::vector<ledgerEntry> entries = {
	{1, 1, salary.id, 1000.0, entryDirection::Credit, "2025-12-08T09:32:03Z", "December salary", entryStatus::Posted},
	{2, 1, groceries.id, 50.0, entryDirection::Debit, "2025-12-10T014:53:08Z", "Groceries for the family", entryStatus::Posted},
	{3, 1, groceries.id, 50.0, entryDirection::Debit, "2025-12-12T05:23:42Z", "Voided groceries", entryStatus::Voided}
};

Ledger ledger;
std::string start = "2025-12-01T00-00-00Z";
std::string end = "2025-12-31T23-59-59Z";

std::cout<<"Balance: " << ledger.balance(my_account, entries, end) << "\n";
std::cout<<"Credits: " << ledger.total_credits(my_account.id, entries, start, end) << "\n";
std::cout<<"Debits: " << ledger.total_debits(my_account.id,entries, start, end) << "\n";
std::cout<<"Category total: " << ledger.total_category(my_account.id, groceries.id, entryDirection::Debit, entries, start, end) << "\n";

return 0;
}


