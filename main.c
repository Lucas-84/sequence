#include "sequence.h"
#include <math.h>
#include <stdio.h>

enum type
{
        TYPE_ARITHMETIC = 1 << 0, 
	TYPE_GEOMETRIC  = 1 << 1
};

struct data 
{
	enum type progression;
	double factor;
};

static enum report analyze_sequence (const double *const sequence,
				     const size_t start,
				     struct data *const data,
				     const double dig)
{
	enum report res = REPORT_NOTHING;

	if (start > 1)
	{
		size_t i;
		const double add_factor = sequence[1] - sequence[0];
		const double mul_factor = sequence[1] / sequence[0];

		data->progression = TYPE_ARITHMETIC | TYPE_GEOMETRIC;

		for (i = 2; i < start; i++)
		{
			const double add_current = sequence[i] - sequence[i - 1];
			const double mul_current = sequence[i] / sequence[i - 1];

			if (fabs (add_current - add_factor) > dig)
			{
				data->progression &= ~TYPE_ARITHMETIC;
			}
			
			if (fabs (mul_current - mul_factor) > dig)
			{
				data->progression &= ~TYPE_GEOMETRIC;
			}
		}

		if ((data->progression & TYPE_ARITHMETIC) != 0)
		{
			if ((data->progression & TYPE_GEOMETRIC) != 0)
			{
				res = REPORT_AMBIGUOUS;
			}
			else
			{
				data->factor = add_factor;
			}
		}
		else if ((data->progression & TYPE_GEOMETRIC) != 0)
		{
			data->factor = mul_factor;
		}
		else
		{
			res = REPORT_FACTOR;
		}
	}
	else
	{
		res = REPORT_AMBIGUOUS;
	}

	return res;
}


enum report complete_sequence (double *const sequence,
			       const size_t start,
			       const size_t gen,
			       const double dig)
{
	enum report res;

	if (sequence != NULL)
	{
		struct data tmp;

		res = analyze_sequence (sequence, start, &tmp, dig);

		if (res == REPORT_NOTHING)
		{
			double current = sequence[start - 1];
			size_t i;

			for (i = 0; i < gen; i++)
			{
				switch (tmp.progression)
				{
				case TYPE_ARITHMETIC:
					current += tmp.factor;
					break;
				case TYPE_GEOMETRIC:
					current *= tmp.factor;
					break;
				}

				sequence[start + i] = current;
			}
		}
	}
	else
	{
		res = REPORT_BADARG;
	}

	return res;
}
