#!/usr/bin/env python3
"""
Magic Square Pattern Visualizer
Analyzes CSV data from magic square candidate analysis to find patterns
that might lead to discovering perfect magic squares of squares.
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np
from pathlib import Path
import argparse
from collections import Counter
import warnings
warnings.filterwarnings('ignore')

class MagicSquareVisualizer:
    def __init__(self, csv_file='pattern_analysis.csv'):
        self.csv_file = csv_file
        self.df = None
        self.load_data()
        
    def load_data(self):
        """Load and preprocess the CSV data"""
        try:
            self.df = pd.read_csv(self.csv_file)
            print(f"Loaded {len(self.df)} candidates from {self.csv_file}")
            
            # Clean and convert data types
            self.df['index'] = pd.to_numeric(self.df['index'])
            self.df['near_miss_error'] = pd.to_numeric(self.df['near_miss_error'])
            self.df['ratio_range'] = pd.to_numeric(self.df['ratio_range'])
            
            # Parse prime factors
            self.df['prime_factor_count'] = self.df['prime_factors'].str.split().str.len()
            self.df['has_5'] = self.df['prime_factors'].str.contains('5')
            self.df['has_13'] = self.df['prime_factors'].str.contains('13')
            self.df['has_17'] = self.df['prime_factors'].str.contains('17')
            self.df['has_41'] = self.df['prime_factors'].str.contains('41')
            self.df['has_349'] = self.df['prime_factors'].str.contains('349')
            
            # Calculate relative error (normalized by index size)
            self.df['relative_error'] = self.df['near_miss_error'] / self.df['index']
            
        except FileNotFoundError:
            print(f"Error: {self.csv_file} not found!")
            return
        except Exception as e:
            print(f"Error loading data: {e}")
            return
    
    def overview_stats(self):
        """Print overview statistics"""
        print("\n" + "="*50)
        print("MAGIC SQUARE CANDIDATE ANALYSIS OVERVIEW")
        print("="*50)
        
        print(f"Total candidates analyzed: {len(self.df)}")
        print(f"Index range: {self.df['index'].min():,} to {self.df['index'].max():,}")
        print(f"Equidistant pairs: {self.df['equidistant_count'].min()} to {self.df['equidistant_count'].max()}")
        print(f"Best near miss error: {self.df['near_miss_error'].min():,}")
        print(f"Worst near miss error: {self.df['near_miss_error'].max():,}")
        
        # Best candidates
        best_candidates = self.df.nsmallest(5, 'near_miss_error')
        print(f"\nTOP 5 BEST NEAR MISSES:")
        for _, row in best_candidates.iterrows():
            print(f"  {row['index']:>8}: error={row['near_miss_error']:>12,}, factors={row['prime_factors'].strip()}")
        
        # Prime factor analysis
        print(f"\nPRIME FACTOR DISTRIBUTION:")
        print(f"  Has factor 5:   {self.df['has_5'].sum()}/{len(self.df)} ({100*self.df['has_5'].mean():.1f}%)")
        print(f"  Has factor 13:  {self.df['has_13'].sum()}/{len(self.df)} ({100*self.df['has_13'].mean():.1f}%)")
        print(f"  Has factor 17:  {self.df['has_17'].sum()}/{len(self.df)} ({100*self.df['has_17'].mean():.1f}%)")
        print(f"  Has factor 41:  {self.df['has_41'].sum()}/{len(self.df)} ({100*self.df['has_41'].mean():.1f}%)")
        print(f"  Has factor 349: {self.df['has_349'].sum()}/{len(self.df)} ({100*self.df['has_349'].mean():.1f}%)")
    
    def plot_error_vs_index(self):
        """Plot near miss error vs index size"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
        
        # Absolute error
        ax1.scatter(self.df['index'], self.df['near_miss_error'], alpha=0.7, c='blue')
        ax1.set_xlabel('Index')
        ax1.set_ylabel('Near Miss Error (absolute)')
        ax1.set_title('Near Miss Error vs Index Size')
        ax1.set_yscale('log')
        ax1.grid(True, alpha=0.3)
        
        # Relative error
        ax2.scatter(self.df['index'], self.df['relative_error'], alpha=0.7, c='red')
        ax2.set_xlabel('Index')
        ax2.set_ylabel('Relative Error (error/index)')
        ax2.set_title('Relative Error vs Index Size')
        ax2.set_yscale('log')
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('error_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def plot_prime_factor_analysis(self):
        """Analyze the impact of prime factors on near miss quality"""
        fig, axes = plt.subplots(2, 3, figsize=(18, 12))
        
        prime_factors = [('has_5', '5'), ('has_13', '13'), ('has_17', '17'), 
                        ('has_41', '41'), ('has_349', '349')]
        
        for i, (col, factor) in enumerate(prime_factors):
            ax = axes[i//3, i%3]
            
            # Box plot comparing error distributions
            has_factor = self.df[self.df[col]]['relative_error']
            no_factor = self.df[~self.df[col]]['relative_error']
            
            ax.boxplot([has_factor, no_factor], labels=[f'Has {factor}', f'No {factor}'])
            ax.set_ylabel('Relative Error')
            ax.set_title(f'Error Distribution: Factor {factor}')
            ax.set_yscale('log')
            ax.grid(True, alpha=0.3)
        
        # Factor count analysis
        ax = axes[1, 2]
        factor_counts = self.df['prime_factor_count'].value_counts().sort_index()
        ax.bar(factor_counts.index, factor_counts.values)
        ax.set_xlabel('Number of Prime Factors')
        ax.set_ylabel('Count')
        ax.set_title('Distribution of Prime Factor Counts')
        ax.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('prime_factor_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def plot_correlation_heatmap(self):
        """Show correlations between different metrics"""
        # Select numeric columns for correlation
        numeric_cols = ['index', 'equidistant_count', 'near_miss_error', 'relative_error',
                       'ratio_range', 'prime_factor_count']
        
        correlation_matrix = self.df[numeric_cols].corr()
        
        plt.figure(figsize=(10, 8))
        sns.heatmap(correlation_matrix, annot=True, cmap='coolwarm', center=0,
                   square=True, linewidths=0.5)
        plt.title('Correlation Matrix: Magic Square Metrics')
        plt.tight_layout()
        plt.savefig('correlation_heatmap.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def plot_ratio_analysis(self):
        """Analyze the ratio patterns in equidistant pairs"""
        fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
        
        # Ratio distribution
        ax1.hist(self.df['ratio_range'], bins=30, alpha=0.7, edgecolor='black')
        ax1.set_xlabel('Ratio Range (largest/smallest pair)')
        ax1.set_ylabel('Frequency')
        ax1.set_title('Distribution of Pair Ratios')
        ax1.grid(True, alpha=0.3)
        
        # Ratio vs error
        ax2.scatter(self.df['ratio_range'], self.df['relative_error'], alpha=0.7)
        ax2.set_xlabel('Ratio Range')
        ax2.set_ylabel('Relative Error')
        ax2.set_title('Pair Ratio vs Near Miss Quality')
        ax2.set_yscale('log')
        ax2.grid(True, alpha=0.3)
        
        plt.tight_layout()
        plt.savefig('ratio_analysis.png', dpi=300, bbox_inches='tight')
        plt.show()
    
    def find_promising_patterns(self):
        """Identify the most promising patterns for magic square discovery"""
        print("\n" + "="*50)
        print("PROMISING PATTERN ANALYSIS")
        print("="*50)
        
        # Best performers by different metrics
        print("BEST CANDIDATES BY RELATIVE ERROR:")
        best_relative = self.df.nsmallest(10, 'relative_error')
        for _, row in best_relative.iterrows():
            print(f"  {row['index']:>8}: rel_error={row['relative_error']:>10.3f}, factors={row['prime_factors'].strip()}")
        
        # Analyze factor combinations
        print(f"\nFACTOR COMBINATION ANALYSIS:")
        
        # Look for patterns in the best candidates
        best_10 = self.df.nsmallest(10, 'relative_error')
        factor_patterns = []
        for _, row in best_10.iterrows():
            factors = row['prime_factors'].strip().split()
            factor_patterns.append(tuple(sorted(factors)))
        
        pattern_counts = Counter(factor_patterns)
        print("Most common factor patterns in top candidates:")
        for pattern, count in pattern_counts.most_common(5):
            print(f"  {pattern}: {count} occurrences")
        
        # Statistical insights
        print(f"\nSTATISTICAL INSIGHTS:")
        print(f"  Mean ratio range: {self.df['ratio_range'].mean():.3f}")
        print(f"  Std ratio range:  {self.df['ratio_range'].std():.3f}")
        print(f"  Best ratio range: {self.df['ratio_range'].min():.3f}")
        
        correlation = self.df['ratio_range'].corr(self.df['relative_error'])
        print(f"  Ratio-Error correlation: {correlation:.3f}")
    
    def generate_candidates_for_large_scale(self):
        """Generate recommendations for the 1000-20million search"""
        print("\n" + "="*50)
        print("RECOMMENDATIONS FOR LARGE-SCALE SEARCH")
        print("="*50)
        
        # Analyze patterns from current data
        best_patterns = self.df.nsmallest(10, 'relative_error')
        
        print("STRATEGIC INSIGHTS:")
        print("1. Focus on numbers with these prime factor combinations:")
        for _, row in best_patterns.iterrows():
            factors = row['prime_factors'].strip()
            print(f"   - Pattern like {factors} (index ~{row['index']})")
        
        print(f"\n2. Target ratio ranges between {self.df['ratio_range'].min():.3f} and {self.df['ratio_range'].quantile(0.25):.3f}")
        print(f"3. Look for indices divisible by: {', '.join(['5', '13', '17', '41'])}")
        
        # Generate some specific candidates
        print(f"\n4. SPECIFIC TARGETS FOR 1000-20M RANGE:")
        base_factors = [5, 13, 17, 41, 349]
        
        print("   Numbers with promising factor combinations:")
        candidates = []
        for i in range(1000, 100000, 100):  # Sample some candidates
            factor_score = sum(1 for f in base_factors if i % f == 0)
            if factor_score >= 3:  # Has at least 3 of our target factors
                candidates.append((i, factor_score))
        
        candidates.sort(key=lambda x: x[1], reverse=True)
        for candidate, score in candidates[:20]:
            print(f"   - {candidate:>6} (factor score: {score})")
    
    def run_full_analysis(self):
        """Run the complete analysis suite"""
        if self.df is None:
            print("No data loaded!")
            return
        
        print("Starting comprehensive magic square pattern analysis...")
        
        # Overview
        self.overview_stats()
        
        # Visualizations
        print("\nGenerating visualizations...")
        self.plot_error_vs_index()
        self.plot_prime_factor_analysis()
        self.plot_correlation_heatmap()
        self.plot_ratio_analysis()
        
        # Pattern analysis
        self.find_promising_patterns()
        self.generate_candidates_for_large_scale()
        
        print("\n" + "="*50)
        print("ANALYSIS COMPLETE!")
        print("Generated files:")
        print("  - error_analysis.png")
        print("  - prime_factor_analysis.png") 
        print("  - correlation_heatmap.png")
        print("  - ratio_analysis.png")
        print("="*50)

def main():
    parser = argparse.ArgumentParser(description='Visualize magic square patterns')
    parser.add_argument('--csv', default='pattern_analysis.csv', 
                       help='CSV file to analyze (default: pattern_analysis.csv)')
    parser.add_argument('--quick', action='store_true',
                       help='Run quick analysis without plots')
    
    args = parser.parse_args()
    
    visualizer = MagicSquareVisualizer(args.csv)
    
    if args.quick:
        visualizer.overview_stats()
        visualizer.find_promising_patterns()
    else:
        visualizer.run_full_analysis()

if __name__ == "__main__":
    main()