int TimetoFreq(int t) {
  if (t > 6000) return 230;
  else if (t > 3000) return int(15000 / sqrt(0.7 * t));
  else if (t > 2000) return int(18000 / sqrt(t));
  else if (t > 1000) return int(-0.4 * t + 1200);
  else if (t > 500) return int(-0.8 * t + 1600);
  else if (t > 250) return int(-1.2 * t + 1800);
  else return 1550;
}
