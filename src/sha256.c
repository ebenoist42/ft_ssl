/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebenoist <ebenoist@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/15 14:24:05 by ebenoist          #+#    #+#             */
/*   Updated: 2026/07/15 14:24:44 by ebenoist         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

#define ROTR(x, c) (((x) >> (c)) | ((x) << (32 - (c))))
#define S0(x) (ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define S1(x) (ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define D0(x) (ROTR(x, 7) ^ ROTR(x, 18) ^ ((x) >> 3))
#define D1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ ((x) >> 10))

static const uint32_t	g_k[64] = {
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

static void	sha256_block(uint32_t st[8], const uint8_t block[64])
{
	uint32_t	w[64];
	uint32_t	v[8];
	uint32_t	t1;
	uint32_t	t2;
	int			i;

	i = 0;
	while (i < 16)
	{
		/* mots lus en BIG-ENDIAN */
		w[i] = ((uint32_t)block[i * 4] << 24)
			| ((uint32_t)block[i * 4 + 1] << 16)
			| ((uint32_t)block[i * 4 + 2] << 8)
			| ((uint32_t)block[i * 4 + 3]);
		i++;
	}
	while (i < 64)
	{
		w[i] = D1(w[i - 2]) + w[i - 7] + D0(w[i - 15]) + w[i - 16];
		i++;
	}
	i = 0;
	while (i < 8)
	{
		v[i] = st[i];
		i++;
	}
	i = 0;
	while (i < 64)
	{
		t1 = v[7] + S1(v[4]) + ((v[4] & v[5]) ^ (~v[4] & v[6])) + g_k[i] + w[i];
		t2 = S0(v[0]) + ((v[0] & v[1]) ^ (v[0] & v[2]) ^ (v[1] & v[2]));
		v[7] = v[6];
		v[6] = v[5];
		v[5] = v[4];
		v[4] = v[3] + t1;
		v[3] = v[2];
		v[2] = v[1];
		v[1] = v[0];
		v[0] = t1 + t2;
		i++;
	}
	i = 0;
	while (i < 8)
	{
		st[i] += v[i];
		i++;
	}
}

static uint8_t	*sha256_pad(const uint8_t *msg, size_t len, size_t *out_len)
{
	uint8_t		*buf;
	uint64_t	bits;
	size_t		padded;
	size_t		i;

	padded = (((len + 8) / 64) + 1) * 64;
	buf = malloc(padded);
	if (!buf)
		return (NULL);
	i = 0;
	while (i < len)
	{
		buf[i] = msg[i];
		i++;
	}
	buf[len] = 0x80;
	i = len + 1;
	while (i < padded - 8)
		buf[i++] = 0;
	bits = (uint64_t)len * 8;
	i = 0;
	while (i < 8)
	{
		/* longueur en bits, sur 64 bits, en BIG-ENDIAN */
		buf[padded - 1 - i] = (uint8_t)(bits >> (8 * i));
		i++;
	}
	*out_len = padded;
	return (buf);
}

void	ft_sha256(const uint8_t *msg, size_t len, uint8_t digest[32])
{
	uint32_t	st[8];
	uint8_t		*buf;
	size_t		padded;
	size_t		off;
	int			i;

	st[0] = 0x6a09e667;
	st[1] = 0xbb67ae85;
	st[2] = 0x3c6ef372;
	st[3] = 0xa54ff53a;
	st[4] = 0x510e527f;
	st[5] = 0x9b05688c;
	st[6] = 0x1f83d9ab;
	st[7] = 0x5be0cd19;
	buf = sha256_pad(msg, len, &padded);
	if (!buf)
		return ;
	off = 0;
	while (off < padded)
	{
		sha256_block(st, buf + off);
		off += 64;
	}
	free(buf);
	i = 0;
	while (i < 8)
	{
		/* etat ressorti en BIG-ENDIAN */
		digest[i * 4] = (uint8_t)(st[i] >> 24);
		digest[i * 4 + 1] = (uint8_t)(st[i] >> 16);
		digest[i * 4 + 2] = (uint8_t)(st[i] >> 8);
		digest[i * 4 + 3] = (uint8_t)st[i];
		i++;
	}
}