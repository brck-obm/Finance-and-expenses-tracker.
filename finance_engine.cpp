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
account my_account{"my_account", 1, accountType::Asset, "2024-01-08T08:29:00Z"};//ISO8601 time (YYYY-MM-DDTHH:mm:SSZ)
account account_2{"account_2", 2, accountType::Liability, "2025-02-19T04:21:33Z"}; //{name, account type, date created}
category salary{1, 1, "Salary", true};
category groceries{2, 1, "Groceries", true};
category electric_bills{3, 1, "Electric Bills", true};
category salary_account2{4, 2, "Salary account_2", true}; //{id, account id, name, is_active}
category arbitrary_amount{5,2,"Arbitrary amount", true};

std::vector<ledgerEntry> entries = {
	{1, 1, salary.id, 1000.0, entryDirection::Credit, "2025-12-08T09:32:03Z", "December salary", entryStatus::Posted},
	{2, 1, groceries.id, 50.0, entryDirection::Debit, "2025-12-10T014:53:08Z", "Groceries for the family", entryStatus::Posted},
	{3, 1, groceries.id, 50.0, entryDirection::Debit, "2025-12-12T05:23:42Z", "Voided groceries", entryStatus::Voided},
	{4, 1, electric_bills.id, 100.0, entryDirection::Debit, "2025-12-26T09:04:23Z", "Electric bills", entryStatus::Posted},
	{5, 2, salary_account2.id, 233.0, entryDirection::Credit, "2025-08-24T02:42:45Z", "salary for me", entryStatus::Posted},
	{6, 2, arbitrary_amount.id, 142.42, entryDirection::Debit, "2025-09-01T23:23:34Z", "Amount taken away", entryStatus::Posted}
};

Ledger ledger;
std::string start = "2025-01-01T00:00:00Z";
std::string end = "2025-12-31T23-59-59Z";

std::cout<<"Balance a : " << ledger.balance(my_account, entries, end) << "\n";
std::cout<<"Credits a : " << ledger.total_credits(my_account.id, entries, start, end) << "\n";
std::cout<<"Debits a : " << ledger.total_debits(my_account.id,entries, start, end) << "\n";
std::cout<<"Category total a : " << ledger.total_category(my_account.id, groceries.id, entryDirection::Debit, entries, start, end) << "\n";
std::cout<<"\n";
std::cout<<"Balance b : " << ledger.balance(account_2, entries, end) << "\n";
std::cout<<"Credits b : " << ledger.total_credits(account_2.id, entries, start, end) << "\n";
std::cout<<"Debits b : " << ledger.total_debits(account_2.id, entries, start, end) << "\n";
std::cout<<"Category total b : " << ledger.total_category(account_2.id, arbitrary_amount.id, entryDirection::Debit, entries, start, end) << "\n";


return 0;
}

//NEXT: Add three dp fixed precision support (multiply user inputs by SF1000, use that for computation, then output divided by SF1000. 
//NEXT: Add formal currency support and actual time objects using chrono 
