#ifndef _HISTORY_TRANSACTION_HPP_
#define _HISTORY_TRANSACTION_HPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project3.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK
//
Transaction::Transaction(std::string ticker_symbol, unsigned int day_date,
              unsigned int month_date, unsigned year_date, bool buy_sell_trans,
              unsigned int number_shares, double trans_amount):
	day{day_date},
	month{month_date},
	year{year_date},
	shares{number_shares},
	amount{trans_amount}

{
	symbol = ticker_symbol;
	if (buy_sell_trans){
		trans_type = "Buy";
	}else{
		trans_type = "Sell";
	}
	trans_id = assigned_trans_id;
	p_next = nullptr;
	acb = 0.0;
	acb_per_share = 0.0;
	cgl = 0.0;
	share_balance = 0;
	assigned_trans_id++;
}//Transaction constructor

// Destructor
// TASK
//
Transaction::~Transaction() {
	p_next = nullptr;
}//Transaction destructor

// TASK
// Overloaded < operator.
//
bool Transaction::operator<(Transaction const &other) {
	//Check year
	if (year < other.get_year()){
	  return true;
	}
	if (year > other.get_year()){
	  return false;
	}
	//check month
	if (month < other.get_month()){
	  return true;
	}
	if (month > other.get_month()){
	  return false;
	}
	//check day
	if (day > other.get_day()){
	  return false;
	}

	return true;
}//Overloaded < operator for Transactions

// GIVEN
// Member functions to get values. 
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values. 
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " " 
    << std::setw(4) << get_symbol() << " " 
    << std::setw(4) << get_day() << " "  
    << std::setw(4) << get_month() << " " 
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) { 
    std::cout << "  Buy  "; 
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " " 
    << std::setw(10) << get_amount() << " " 
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl() 
    << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK
//
History::History():
	p_head{nullptr}
{}//History constructor

// Destructor
// TASK
//
History::~History() {
	//Deallocate list
	while (p_head != nullptr){
		//Delete the current list head and move the list head
		//to the next instance in the list
		Transaction *p_copy{p_head};
		p_head = p_copy->get_next();
		delete p_copy;
	}//while
}//History destructor

// TASK
// read_transaction(...): Read the transaction history from file. 
//
void History::read_history() {
	ece150::open_file();

	//Assign p_head
	ece150::next_trans_entry();
	p_head = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(),
			ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(),
			ece150::get_trans_shares(), ece150::get_trans_amount());

	//While not at the end of the file
	while(ece150::next_trans_entry()){
		//Add a new instance to the linked list
		History::insert((new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(),
				ece150::get_trans_month(), ece150::get_trans_year(), ece150::get_trans_type(),
				ece150::get_trans_shares(), ece150::get_trans_amount())));
	}

	ece150::close_file();
}//read_history

// TASK
// insert(...): Insert transaction into linked list.
//
void History::insert(Transaction *p_new_trans) {
	//Find the end of the list
	Transaction *p_temp{p_head};
	while (p_temp->get_next() != nullptr){
		p_temp = p_temp->get_next();
	}
	//Insert transaction at the end of the list
	p_temp->set_next(p_new_trans);
}//Insert


// TASK
// sort_by_date(): Sort the linked list by trade date.
//
void History::sort_by_date() {
	//Check if list is empty
	if (p_head == nullptr){
		return;
	}
	Transaction *p_pos{p_head};	//end of the sorted list

	//Loop until at the end of the list
	while (p_pos->get_next() != nullptr){
		//If the next instance is less than the current one, remove the next intance from the list
		if (*p_pos->get_next() < *p_pos){
			Transaction *p_temp{p_pos->get_next()};		//The removed instance
			p_pos->set_next(p_temp->get_next());

			//Compare p_temp with the sorted list (starting from the beginning of the list)
			if (*p_head < *p_temp){
				Transaction *p_count{p_head};
				while(p_count->get_next() != p_pos && *p_count->get_next() < *p_temp){
				  p_count = p_count->get_next();
				}
				//Insert p_temp in the list
				p_temp->set_next(p_count->get_next());
				p_count->set_next(p_temp);
			}else{
				p_temp->set_next(p_head);
				p_head = p_temp;
			}
		}else{
			//If the list is already sorted, move the end of the sorted list down
			p_pos = p_pos->get_next();
		}
	}//while
}//sort

// TASK
// update_acb_cgl(): Updates the ACB and CGL values. 
//
void History::update_acb_cgl() {
	Transaction *p_temp{p_head};		//End of the updated list
	double sum_acb{0};				//Total acb to date
	unsigned int sum_shares{0};		//Total number of shares to date
	double previous_acbps{0};			//acb per share of the previous instance

	//Loop until the end of list
	while (p_temp != nullptr){
		//Check if the transaction was a buy or sell
		if (p_temp->get_trans_type()){
		  //Add shares and acb to sum
		  sum_acb += p_temp->get_amount();
		  sum_shares += p_temp->get_shares();
		}else{
			//Calculate cgl and subtract acb accordingly
			//Change is acb is proportional to acb per share and number of shares
			sum_acb -= (previous_acbps*p_temp->get_shares());
			sum_shares -= p_temp->get_shares();
			//cgl is the difference between the change in acb and amount that the shares were sold for
			p_temp->set_cgl(p_temp->get_amount() - previous_acbps*p_temp->get_shares());
		}
		//Assign updated values
		p_temp->set_acb(sum_acb);
		p_temp->set_share_balance(sum_shares);
		p_temp->set_acb_per_share(sum_acb/sum_shares);
		previous_acbps = p_temp->get_acb_per_share();

		//Move on to the next instance
		p_temp = p_temp->get_next();
	}//while not at the end of the list
}//update_acb_cgl

// TASK
// compute_cgl(): )Compute the ACB, and CGL.
//
double History::compute_cgl(unsigned int year) {
	Transaction *p_temp{p_head};		//Current position in the linked list
	double sum_cgl{0};					//Total cgl of the year

	//Loop until p_temp is at the right year
	while(p_temp != nullptr && p_temp->get_year() < year){
		p_temp = p_temp->get_next();
	}
	//Loop while p_temp is still at the right year
	while(p_temp != nullptr && p_temp->get_year() == year){
		//If the current instance has a cgl, add the cgl to the total
		if (!p_temp->get_trans_type()){
		  sum_cgl += p_temp->get_cgl();
		}
		p_temp = p_temp->get_next();
	}

	return sum_cgl;
 }//compute_cgl

// TASK
// print() Print the transaction history.
//
void History::print() {
	Transaction *p_temp{p_head};		//Current position in the linked list

	std::cout << "========== BEGIN TRANSACTION HISTORY ============" << std::endl;
	//Loop until at the end of the linked list
	while (p_temp != nullptr){
		//Print each individual transaction and move the p_temp to the next instance
		p_temp->print();
		p_temp = p_temp->get_next();
	}
	std::cout << "========== END TRANSACTION HISTORY ============" << std::endl;
}//print

// GIVEN
// get_p_head(): Full access to the linked list.
// 
Transaction *History::get_p_head() { return p_head; }

#endif

