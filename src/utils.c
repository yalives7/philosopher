// utils.c
#include "philo.h"

// timeval yapısını milisaniyeye çevirir
static long long	convert_timeval_to_ms(struct timeval tv)
{
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000));
}

// Şu anki zaman damgasını milisaniye cinsinden alır
long long	get_timestamp_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (convert_timeval_to_ms(tv));
}

// Hassas milisaniye sleep - referans projeden esinlenildi
void	smart_sleep(int duration_ms, t_data *data)
{
	long long	start_time;
	long long	current_time;
	long long	elapsed;

	if (duration_ms <= 0)
		return ;
	start_time = get_timestamp_ms();
	while (1)
	{
		// İlk simülasyon bitiş kontrolü
		if (data && has_simulation_ended(data))
			break ;
		
		current_time = get_timestamp_ms();
		elapsed = current_time - start_time;
		if (elapsed >= duration_ms)
			break ;
		
		// Çok kısa sleep aralıkları - daha responsive
		usleep(500); // 0.5ms
		
		// Her sleep sonrası tekrar kontrol
		if (data && has_simulation_ended(data))
			break ;
	}
}

// Simülasyonun bitip bitmediğini kontrol eder (mutex ile)
int	has_simulation_ended(t_data *data)
{
	int	ended;

	pthread_mutex_lock(&data->death_mutex);
	ended = data->simulation_end;
	pthread_mutex_unlock(&data->death_mutex);
	return (ended);
}

// Filozofun yaptığı eylemi zaman damgası ile ekrana yazdırır
void	print_philo_action(t_philo *philo, const char *status)
{
	long long	time_now;

	pthread_mutex_lock(&philo->data->print_mutex);
	// Simülasyon bitmişse yazdırma
	if (!has_simulation_ended(philo->data))
	{
		time_now = get_timestamp_ms() - philo->data->start_time;
		printf("%lld %d %s\n", time_now, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}
