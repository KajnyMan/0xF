unsigned long int power10( int exp )
{
	if ( exp == 0 ) return 1;
	return 10 * power10( exp - 1);
}
int power( int d, int pow )
{
	if ( pow <= 0 ) return 1;
	return ( d * power( d, pow - 1 ) );
}
