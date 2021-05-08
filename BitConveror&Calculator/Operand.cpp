#include "Operand.h"

Operand::Operand(string number)
{
	char sign = number.at(0);

	if (sign == 'b')
	{
		number.erase(0, 1);
		value = new Binary(number);
	}
	else if (sign == 'x')
	{
		number.erase(0, 1);
		value = new HexaDecimal(number);
	}
	else
	{
		value = new Decimal(number);
	}

	priority = 3;
}

void Operand::View()
{
}